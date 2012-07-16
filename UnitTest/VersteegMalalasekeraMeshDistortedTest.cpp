#include "VersteegMalalasekeraMeshDistortedTest.h"

#include "FiniteVolume2DLib/ASCIIMeshReader.h"
#include "FiniteVolume2DLib/Math.h"
#include "FiniteVolume2DLib/MeshChecker.h"
#include "FiniteVolume2DLib/LineSegment.h"
#include "FiniteVolume2DLib/Ray.h"
#include "FiniteVolume2DLib/GeometricHelper.h"

#include "FiniteVolume2D/ComputationalMeshBuilder.h"
#include "FiniteVolume2D/ComputationalMeshSolverHelper.h"
#include "FiniteVolume2D/IComputationalGridAccessor.h"

#include <boost/filesystem.hpp>

#include <algorithm>

namespace FS = boost::filesystem;


// Static class data members
MeshBuilderMock                                VersteegMalalasekeraMeshDistortedTest::mesh_builder_;
VersteegMalalasekeraMeshDistortedTest::MeshPtr VersteegMalalasekeraMeshDistortedTest::mesh_;
BoundaryConditionCollection                    VersteegMalalasekeraMeshDistortedTest::bc_;


#pragma warning(disable:4482)


void
VersteegMalalasekeraMeshDistortedTest::setUp() {
    mesh_filename_ = "Data\\Versteeg_Malalasekera_Mesh_Distorted_11_25.mesh";

    initMesh();
}

void
VersteegMalalasekeraMeshDistortedTest::tearDown() {
}

void
VersteegMalalasekeraMeshDistortedTest::testMeshFileExists() {
    bool exists = FS::exists(mesh_filename_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mesh file not found!", true, exists);
}

namespace {
  
    double
    checkFluxBalance(IComputationalMesh const & cmesh, std::string const & cvar_name) {
        /* Check that the inflow flux is the same as the outflow flux, i.e.
         * the fluxes have to balance across the boundary.
         * Note that the internal face fluxes are balanced by construction.
         * This necessitates a consistent method when evaluating face fluxes!
         */
        Thread<ComputationalFace> bf = cmesh.getFaceThread(IGeometricEntity::BOUNDARY);

        // total flux balance
        double flux = 0;

        std::for_each(bf.begin(), bf.end(), [&flux, &cvar_name](ComputationalFace::Ptr const & cface){
        
            FluxComputationalMolecule const & flux_molecule = cface->getComputationalMolecule(cvar_name);
            ComputationalCell::Ptr const & ccell = flux_molecule.getCell();
            BoundaryCondition::Ptr const & bc = cface->getBoundaryCondition();

            if (bc->type() == BoundaryConditionCollection::DIRICHLET) {
                // compute face mid point
                Vertex midpoint = (cface->startNode().location() + cface->endNode().location()) / 2.0;

                // distance from face midpoint to the cell centroid
                double dist = Math::dist(ccell->centroid(), midpoint);

                ComputationalMolecule const & cell_molecule = ccell->getComputationalMolecule(cvar_name);


                // see Versteeg, Malalasekera, p. 331-334
                double T_face = bc->getValue();
                double T_P = cell_molecule.getValue();
                double face_flux = (T_face - T_P) / dist * cface->area();

                flux += face_flux;
            }
            else {
                // for von Neumann b.c., the face flux is given directly
                flux += bc->getValue();
            }
        });

        return flux;
    }

    double
    cell_skewness(ComputationalCell::Ptr const & c1, ComputationalCell::Ptr const & c2, ComputationalFace::Ptr const & cface) {
        /* Compute the distance of the line (centroid c1, centroid c2)
         * on face cface from cface's midpoint.
         * cface must be an interior face.
         */
        if (cface->getEntityType() == IGeometricEntity::BOUNDARY)
            return 0;

        Vertex c1_centroid = c1->centroid();
        Vertex c2_centroid = c2->centroid();

        /* Compute the intersection point between the line segment connecting
         * the cell centroids and the face vertices.
         */
        Ray cell_centroid_ls(c1_centroid, c2_centroid);
        LineSegment face_ls(cface->startNode().geometricEntity()->location(), cface->endNode().geometricEntity()->location());

        boost::optional<Vertex> ip_opt = GeometricHelper::intersect(face_ls, cell_centroid_ls);





// 
// 
//         // distance from face midpoint to the cell centroid
//         double dist = Math::dist(cell_centroid, cell_nbr_centroid);
// 
//         /* The weight for the computational molecule is
//          * \gamma f_{area} / dist(N - P).
//          */
//         double weight = cface->area() / dist;
// 
// 
//         // compute face mid point
//         Vertex midpoint = (cface->startNode().location() + cface->endNode().location()) / 2.0;
// 
//         // distance from face midpoint to the cell centroid
//         
//         
//         
//         double dist = Math::dist(ccell->centroid(), midpoint);
        return -1.0;
    }

    bool
    flux_evaluator(IComputationalGridAccessor const & cgrid, ComputationalCell::Ptr const & ccell, ComputationalFace::Ptr const & cface)
    {
        /* Compute flux through a cell face. The cell face may be a boundary face
         * needing special treatment depending on whether Dirichlet or von Neumann
         * boundary conditions have been prescribed.
         * 
         * Flux through an internal face is evaluated by the usual gradient approximation.
         * Note that this is not 2nd order, not even is there is no cell skewness as we
         * omit the cross-diffusion term (Comp. Fluid Dynamics, Versteeg, p. 319).
         * 
         * Note that we are assuming that the face normal and the vector connecting
         * the two cell centers are aligned, i.e. that the mesh is fully orthogonal.
         */

        // Get face flux molecules for "Temperature"
        FluxComputationalMolecule & flux_molecule = cface->getComputationalMolecule("Temperature");

        // Flux through face already computed?
        if (!flux_molecule.empty())
            return true;

        /* We need to know the cell the flux was calculated with. This is because
         * once the neighboring cell accesses the flux, the need to invert the sign.
         */
        flux_molecule.setCell(ccell);

        BoundaryCondition::Ptr const & bc = cface->getBoundaryCondition();

        if (bc) {
            if (bc->type() == BoundaryConditionCollection::DIRICHLET) {
                /* The boundary condition is of type Dirichlet. Compute the
                 * face flux using the half-cell approximation.
                 * Versteeg, p. 331
                 */
                double value = bc->getValue();

                SourceTerm & face_source = flux_molecule.getSourceTerm();

                // compute face mid point
                Vertex midpoint = (cface->startNode().location() + cface->endNode().location()) / 2.0;

                // distance from face midpoint to the cell centroid
                double dist = Math::dist(ccell->centroid(), midpoint);

                // the boundary value contributes as a source term
                face_source += cface->area() / dist * value;

                // get comp. variable to solve for
                ComputationalVariable::Ptr const & cvar = ccell->getComputationalVariable("Temperature");
                flux_molecule.add(*cvar, -cface->area() / dist);
            }
            else {
                // Face b.c. given as von Neumann
                SourceTerm & face_source = flux_molecule.getSourceTerm();

                // the boundary flux contributes as a source term
                face_source += bc->getValue();
            }
            return true;
        }

        // internal face


        // Compute distance to face midpoint to monitor accuracy

    
        /* We have to approximate the term
         * <\hat{n}, \gamma \grad \phi f_{area}>
         * compute the usual gradient approximation,
         * \grad \phi \approx \frac{phi_{N} - phi_{P}}{\dist N - P}
         */
        Vertex cell_centroid = ccell->centroid();

        ComputationalCell::Ptr const & cell_nbr = cgrid.getOtherCell(cface, ccell);
        Vertex cell_nbr_centroid = cell_nbr->centroid();

        // distance from face midpoint to the cell centroid
        double dist = Math::dist(cell_centroid, cell_nbr_centroid);

        /* The weight for the computational molecule is
         * \gamma f_{area} / dist(N - P).
         */
        double weight = cface->area() / dist;
    
        // get comp. variable to solve for
        ComputationalVariable::Ptr const & cvar = ccell->getComputationalVariable("Temperature");
        ComputationalVariable::Ptr const & cvar_nbr = cell_nbr->getComputationalVariable("Temperature");

        flux_molecule.add(*cvar, -weight);
        flux_molecule.add(*cvar_nbr, weight);

        return true;
    }

    bool cell_evaluator(ComputationalCell::Ptr const & ccell) {
        EntityCollection<ComputationalFace> const & cface_coll = ccell->getComputationalFaces();

        // Get cell computational molecule for "Temperature"
        ComputationalMolecule & cmolecule = ccell->getComputationalMolecule("Temperature");

        std::for_each(cface_coll.begin(), cface_coll.end(), [&](ComputationalFace::Ptr const & cface) {
            // Get face flux molecules for "Temperature"
            FluxComputationalMolecule & flux_molecule = cface->getComputationalMolecule("Temperature");

            /* negate the weights if they were calculated with the neighboring
             * cell
             */
            ComputationalCell::Ptr const & ccell_ref = flux_molecule.getCell();
            if (ccell_ref != ccell)
                flux_molecule = -flux_molecule;

            cmolecule += flux_molecule;
        });

        // account for r.h.s.
        SourceTerm & st = cmolecule.getSourceTerm();
        st  += -2.0;

        return true;
    }

}



void
VersteegMalalasekeraMeshDistortedTest::checkFluxBalanceTest() {
    ComputationalMeshBuilder builder(mesh_, bc_);

    // Temperature as cell-centered variable, will be solved for
    builder.addComputationalVariable("Temperature", flux_evaluator);
    builder.addEvaluateCellMolecules(cell_evaluator);
    ComputationalMesh::CPtr cmesh(builder.build());


    ComputationalMeshSolverHelper helper(*cmesh);
    CPPUNIT_ASSERT_MESSAGE("Could not solve for computational mesh", helper.solve());

    double flux_balance = checkFluxBalance(*cmesh, "Temperature");
    CPPUNIT_ASSERT_MESSAGE("Flux balance error", std::fabs(flux_balance) < 1E-10);
}

void
VersteegMalalasekeraMeshDistortedTest::initMesh() {
    static bool init = false;
    if (!init)
    {
        init = true;
        ASCIIMeshReader reader(mesh_filename_, mesh_builder_);
        CPPUNIT_ASSERT_MESSAGE("Failed to read mesh file!", reader.read());

        boost::optional<Mesh::Ptr> mesh = mesh_builder_.getMesh();
        if (mesh)
            mesh_ = *mesh;
        CPPUNIT_ASSERT_MESSAGE("Failed to build mesh!", mesh);

        bc_ = reader.getBoundaryConditions();

        bool success = MeshChecker::checkMesh(*mesh, bc_);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Mesh check failed!", true, success);
    }
}
