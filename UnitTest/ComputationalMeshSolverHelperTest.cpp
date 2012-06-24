#include "ComputationalMeshSolverHelperTest.h"

#include "FiniteVolume2D/ComputationalMeshSolverHelper.h"

#include "FiniteVolume2DLib/ASCIIMeshReader.h"
#include "FiniteVolume2DLib/Math.h"
#include "FiniteVolume2DLib/MeshChecker.h"

#include "FiniteVolume2D/ComputationalMeshBuilder.h"
#include "FiniteVolume2D/IComputationalGridAccessor.h"

#include <boost/filesystem.hpp>

namespace FS = boost::filesystem;


// Static class data members
MeshBuilderMock                            ComputationalMeshSolverHelperTest::mesh_builder_;
ComputationalMeshSolverHelperTest::MeshPtr ComputationalMeshSolverHelperTest::mesh_;
BoundaryConditionCollection                ComputationalMeshSolverHelperTest::bc_;


void
ComputationalMeshSolverHelperTest::setUp() {
    mesh_filename_ = "Data\\Versteeg_Malalasekera_11_25.mesh";

    initMesh();
}

void
ComputationalMeshSolverHelperTest::tearDown() {}


namespace {
  
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

            cmolecule.addMolecule(flux_molecule);
        });

        // account for r.h.s.
        SourceTerm & st = cmolecule.getSourceTerm();
        st  += -2.0;

        return true;
    }

}

void
ComputationalMeshSolverHelperTest::matrixConstructionTest() {
    ComputationalMeshBuilder builder(mesh_, bc_);

    // Temperature as cell-centered variable, will be solved for
    builder.addComputationalVariable("Temperature", flux_evaluator);
    builder.addEvaluateCellMolecules(cell_evaluator);
    ComputationalMesh::CPtr cmesh(builder.build());


    ComputationalMeshSolverHelper helper(*cmesh);
    helper.solve();
//    helper.setupMatrix();
    IMatrix2D const & m = helper.getMatrix();
    LinearSolver::RHS_t const & rhs = helper.getRHS();





}

void
ComputationalMeshSolverHelperTest::initMesh() {
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
