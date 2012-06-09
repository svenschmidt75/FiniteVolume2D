#include "VersteegMalalasekeraTest.h"

#include "FiniteVolume2DLib/ASCIIMeshReader.h"
#include "FiniteVolume2DLib/Math.h"
#include "FiniteVolume2DLib/MeshChecker.h"

#include "FiniteVolume2D/ComputationalMeshBuilder.h"
#include "FiniteVolume2D/IComputationalGridAccessor.h"

#include <boost/filesystem.hpp>

namespace FS = boost::filesystem;


// Static class data members
MeshBuilderMock                   VersteegMalalasekeraTest::mesh_builder_;
VersteegMalalasekeraTest::MeshPtr VersteegMalalasekeraTest::mesh_;
BoundaryConditionCollection       VersteegMalalasekeraTest::bc_;


#pragma warning(disable:4482)


void
VersteegMalalasekeraTest::setUp() {
    mesh_filename_ = "Data\\Versteeg_Malalasekera_11_25.mesh";

    initMesh();
}

void
VersteegMalalasekeraTest::tearDown() {
}

void
VersteegMalalasekeraTest::testMeshFileExists() {
    bool exists = FS::exists(mesh_filename_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mesh file not found!", true, exists);
}

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

}

void
VersteegMalalasekeraTest::evaluateFluxesTest() {
    ComputationalMeshBuilder builder(mesh_, bc_);

    // Temperature as cell-centered variable, will be solved for
    builder.addComputationalVariable("Temperature", flux_evaluator);
    ComputationalMesh::Ptr cmesh(builder.build());




    /*
     * Face 6 is a boundary face with b.c. Dirichlet = 0.9987
     */
    auto boundary_face_thread_ = cmesh->getFaceThread(IGeometricEntity::Entity_t::BOUNDARY);

    auto it = std::find_if(boundary_face_thread_.begin(), boundary_face_thread_.end(), [](ComputationalFace::Ptr const & cface) -> bool {
        Face::Ptr const & face = cface->geometricEntity();
        return face->meshId() == 6u;
    });

    CPPUNIT_ASSERT_MESSAGE("Face not found", it != boundary_face_thread_.end());
    ComputationalFace::Ptr cface = *it;

    BoundaryCondition::Ptr bc = cface->getBoundaryCondition();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected Dirichlet boundary conditions", BoundaryConditionCollection::DIRICHLET, bc->type());
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Boundary condition value error", 0.9987, bc->getValue(), 1E-10);

    // check flux computation across boundary faces
    FluxComputationalMolecule fm = cface->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("One contribution expected to boundary face flux", 1u, fm.size());

    // check that the face flux was computed with the correct cell
    ComputationalCell::Ptr ccell = fm.getCell();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong computational cell used for face flux calculation", 2ull, ccell->geometricEntity()->meshId());

    // check the correct computational variable is used in the comp. face molecule
    ComputationalVariable::Ptr cvar = ccell->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong computational variable in face comp. molecule", -2.6832815729997472, *fm.getWeight(*cvar), 1E-10);

    // check that the source term is correct
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong source term value", 2.6797933069548479, fm.getSourceTerm().value(), 1E-10);


    /*
     * Face 11 is a boundary face with b.c. Dirichlet = 0
     */
    it = std::find_if(boundary_face_thread_.begin(), boundary_face_thread_.end(), [](ComputationalFace::Ptr const & cface) -> bool {
        Face::Ptr const & face = cface->geometricEntity();
        return face->meshId() == 11u;
    });

    CPPUNIT_ASSERT_MESSAGE("Face not found", it != boundary_face_thread_.end());
    cface = *it;

    bc = cface->getBoundaryCondition();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected Dirichlet boundary conditions", BoundaryConditionCollection::DIRICHLET, bc->type());
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Boundary condition value error", 0.0, bc->getValue(), 1E-10);

    // check flux computation across boundary faces
    fm = cface->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("One contribution expected to boundary face flux", 1u, fm.size());

    // check that the face flux was computed with the correct cell
    ccell = fm.getCell();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong computational cell used for face flux calculation", 7ull, ccell->geometricEntity()->meshId());

    // check the correct computational variable is used in the comp. face molecule
    cvar = ccell->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong computational variable in face comp. molecule", -2.6832815729997472, *fm.getWeight(*cvar), 1E-10);

    // check that the source term is correct
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong source term value", 0.0, fm.getSourceTerm().value(), 1E-10);



    
    /*
     * Face 3 is a internal face
     */
    auto internal_face_thread_ = cmesh->getFaceThread(IGeometricEntity::Entity_t::INTERIOR);

    it = std::find_if(internal_face_thread_.begin(), internal_face_thread_.end(), [](ComputationalFace::Ptr const & cface) -> bool {
        Face::Ptr const & face = cface->geometricEntity();
        return face->meshId() == 3u;
    });

    CPPUNIT_ASSERT_MESSAGE("Face not found", it != internal_face_thread_.end());
    cface = *it;

    bc = cface->getBoundaryCondition();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected no boundary conditions for internal face", BoundaryCondition::Ptr(), bc);

    // check flux computation across boundary faces
    fm = cface->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Two contributions expected to interior face flux", 2u, fm.size());

    // check that the face flux was computed with the correct cell
    ccell = fm.getCell();
    CPPUNIT_ASSERT_MESSAGE("Wrong computational cell used for face flux calculation", ccell->geometricEntity()->meshId() == 5 || ccell->geometricEntity()->meshId() == 6);

    // check the correct computational variable is used in the comp. face molecule
    cvar = ccell->getComputationalVariable("Temperature");
    Cell::Ptr cother = cmesh->getMeshConnectivity().getOtherCell(cface->geometricEntity(), ccell->geometricEntity());
    ComputationalCell::Ptr ccother = cmesh->getMapper().getComputationalCell(cother);
    ComputationalVariable::Ptr ccother_var = ccother->getComputationalVariable("Temperature");
    double w1 = *fm.getWeight(*cvar);
    double w2 = *fm.getWeight(*ccother_var);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong computational variable in face comp. molecule", 1.5, std::abs(w1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Weights expected to be opposite in value", w1, -w2, 1E-10);

    // check that the source term is correct
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong source term value", 0.0, fm.getSourceTerm().value(), 1E-10);
}

void
VersteegMalalasekeraTest::initMesh() {
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

#pragma warning(default:4251)
