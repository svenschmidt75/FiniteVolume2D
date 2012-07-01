#include "VersteegMalalasekeraTest.h"

#include "FiniteVolume2DLib/ASCIIMeshReader.h"
#include "FiniteVolume2DLib/Math.h"
#include "FiniteVolume2DLib/MeshChecker.h"

#include "FiniteVolume2D/ComputationalMeshBuilder.h"
#include "FiniteVolume2D/IComputationalGridAccessor.h"

#include <boost/filesystem.hpp>

#include <algorithm>

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
VersteegMalalasekeraTest::evaluateFluxesCell1Test() {
    ComputationalMeshBuilder builder(mesh_, bc_);

    // Temperature as cell-centered variable, will be solved for
    builder.addComputationalVariable("Temperature", flux_evaluator);
    builder.addEvaluateCellMolecules(cell_evaluator);
    ComputationalMesh::CPtr cmesh(builder.build());


    auto boundary_face_thread = cmesh->getFaceThread(IGeometricEntity::Entity_t::BOUNDARY);
    auto internal_face_thread = cmesh->getFaceThread(IGeometricEntity::Entity_t::INTERIOR);


    /* 
     * Check the face fluxes of cell 1
     */

    /* check the flux of boundary face 2 */
    auto it = std::find_if(boundary_face_thread.begin(), boundary_face_thread.end(), [](ComputationalFace::Ptr const & cface) -> bool {
        Face::Ptr const & face = cface->geometricEntity();
        return face->meshId() == 2ull;
    });

    CPPUNIT_ASSERT_MESSAGE("Face 2 not found", it != boundary_face_thread.end());
    ComputationalFace::Ptr cface = *it;

    BoundaryCondition::Ptr bc = cface->getBoundaryCondition();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected Dirichlet boundary conditions", BoundaryConditionCollection::DIRICHLET, bc->type());
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Boundary condition value error", 500, bc->getValue(), 1E-10);

    // check flux computation across boundary face
    FluxComputationalMolecule fm = cface->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("One contribution expected to boundary face flux", 1u, fm.size());

    // check that the face flux was computed with the correct cell
    ComputationalCell::Ptr ccell = fm.getCell();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong computational cell used for face flux calculation", 0ull, ccell->geometricEntity()->meshId());

    // check the correct computational variable is used in the comp. face molecule
    ComputationalVariable::Ptr cvar = ccell->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong computational variable in face comp. molecule", -3.7210420376762530, *fm.getWeight(*cvar), 1E-10);

    // check that the source term is correct
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong source term value", 1860.5210188381266, fm.getSourceTerm().value(), 1E-10);




    /* check the flux of internal face 0 */
    it = std::find_if(internal_face_thread.begin(), internal_face_thread.end(), [](ComputationalFace::Ptr const & cface) -> bool {
        Face::Ptr const & face = cface->geometricEntity();
        return face->meshId() == 0ull;
    });

    CPPUNIT_ASSERT_MESSAGE("Face 0 not found", it != internal_face_thread.end());
    cface = *it;

    bc = cface->getBoundaryCondition();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected no boundary conditions for internal face", BoundaryCondition::Ptr(), bc);

    fm = cface->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Two contributions expected to interior face flux", 2u, fm.size());

    // check that the face flux was computed with the correct cell
    ccell = fm.getCell();
    CPPUNIT_ASSERT_MESSAGE("Wrong computational cell used for face flux calculation", ccell->geometricEntity()->meshId() == 0ull || ccell->geometricEntity()->meshId() == 1ull);

    // check the correct computational variable is used in the comp. face molecule
    cvar = ccell->getComputationalVariable("Temperature");
    Cell::Ptr cother = cmesh->getMeshConnectivity().getOtherCell(cface->geometricEntity(), ccell->geometricEntity());
    ComputationalCell::Ptr ccother = cmesh->getMapper().getComputationalCell(cother);

    IGeometricEntity::Id_t ccother_id = ccell->geometricEntity()->meshId() == 0ull ? 1ull : 0ull;
    CPPUNIT_ASSERT_MESSAGE("Wrong neighbor cell to face 0", ccother->geometricEntity()->meshId() == ccother_id);

    ComputationalVariable::Ptr ccother_var = ccother->getComputationalVariable("Temperature");
    double w1 = *fm.getWeight(*cvar);
    double w2 = *fm.getWeight(*ccother_var);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong weight", 1.8605210188381265, std::abs(w1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Weights expected to be opposite in value", w1, -w2, 1E-10);

    // check that the source term is correct
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong source term value", 0.0, fm.getSourceTerm().value(), 1E-10);




    /* check the flux of internal face 1 */
    it = std::find_if(internal_face_thread.begin(), internal_face_thread.end(), [](ComputationalFace::Ptr const & cface) -> bool {
        Face::Ptr const & face = cface->geometricEntity();
        return face->meshId() == 1ull;
    });

    CPPUNIT_ASSERT_MESSAGE("Face 1 not found", it != internal_face_thread.end());
    cface = *it;

    bc = cface->getBoundaryCondition();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected no boundary conditions for internal face", BoundaryCondition::Ptr(), bc);

    fm = cface->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Two contributions expected to interior face flux", 2u, fm.size());

    // check that the face flux was computed with the correct cell
    ccell = fm.getCell();
    CPPUNIT_ASSERT_MESSAGE("Wrong computational cell used for face flux calculation", ccell->geometricEntity()->meshId() == 0 || ccell->geometricEntity()->meshId() == 7);

    // check the correct computational variable is used in the comp. face molecule
    cvar = ccell->getComputationalVariable("Temperature");
    cother = cmesh->getMeshConnectivity().getOtherCell(cface->geometricEntity(), ccell->geometricEntity());
    ccother = cmesh->getMapper().getComputationalCell(cother);

    ccother_id = ccell->geometricEntity()->meshId() == 0ull ? 7ull : 0ull;
    CPPUNIT_ASSERT_MESSAGE("Wrong neighbor cell to face 0", ccother->geometricEntity()->meshId() == ccother_id);

    ccother_var = ccother->getComputationalVariable("Temperature");
    w1 = *fm.getWeight(*cvar);
    w2 = *fm.getWeight(*ccother_var);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong weight", 1.5, std::abs(w1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Weights expected to be opposite in value", w1, -w2, 1E-10);

    // check that the source term is correct
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong source term value", 0.0, fm.getSourceTerm().value(), 1E-10);
}

void
VersteegMalalasekeraTest::evaluateFluxesCell3Test() {
    ComputationalMeshBuilder builder(mesh_, bc_);

    // Temperature as cell-centered variable, will be solved for
    builder.addComputationalVariable("Temperature", flux_evaluator);
    builder.addEvaluateCellMolecules(cell_evaluator);
    ComputationalMesh::CPtr cmesh(builder.build());


    auto boundary_face_thread = cmesh->getFaceThread(IGeometricEntity::Entity_t::BOUNDARY);
    auto internal_face_thread = cmesh->getFaceThread(IGeometricEntity::Entity_t::INTERIOR);


    /* 
     * Check the face fluxes of cell 4
     */

    /* check the flux of boundary face 7 */
    auto it = std::find_if(boundary_face_thread.begin(), boundary_face_thread.end(), [](ComputationalFace::Ptr const & cface) -> bool {
        Face::Ptr const & face = cface->geometricEntity();
        return face->meshId() == 7ull;
    });

    CPPUNIT_ASSERT_MESSAGE("Face 7 not found", it != boundary_face_thread.end());
    ComputationalFace::Ptr cface = *it;

    BoundaryCondition::Ptr bc = cface->getBoundaryCondition();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected Dirichlet boundary conditions", BoundaryConditionCollection::NEUMANN, bc->type());
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Boundary condition value error", 0, bc->getValue(), 1E-10);

    // since this boundary face has von Neumann b.c., the face flux is directly
    // given and hence only contributes to the source term
    FluxComputationalMolecule fm = cface->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("No contribution expected to boundary face flux", 0u, fm.size());

    // check that the face flux was computed with the correct cell
    ComputationalCell::Ptr ccell = fm.getCell();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong computational cell used for face flux calculation", 3ull, ccell->geometricEntity()->meshId());

    // check that the source term is correct
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong source term value", 0, fm.getSourceTerm().value(), 1E-10);




    /* check the flux of internal face 8 */
    it = std::find_if(internal_face_thread.begin(), internal_face_thread.end(), [](ComputationalFace::Ptr const & cface) -> bool {
        Face::Ptr const & face = cface->geometricEntity();
        return face->meshId() == 8ull;
    });

    CPPUNIT_ASSERT_MESSAGE("Face 8 not found", it != internal_face_thread.end());
    cface = *it;

    bc = cface->getBoundaryCondition();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected no boundary conditions for internal face", BoundaryCondition::Ptr(), bc);

    fm = cface->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Two contributions expected to interior face flux", 2u, fm.size());

    // check that the face flux was computed with the correct cell
    ccell = fm.getCell();
    CPPUNIT_ASSERT_MESSAGE("Wrong computational cell used for face flux calculation", ccell->geometricEntity()->meshId() == 3ull || ccell->geometricEntity()->meshId() == 4ull);

    // check the correct computational variable is used in the comp. face molecule
    ComputationalVariable::Ptr cvar = ccell->getComputationalVariable("Temperature");
    Cell::Ptr cother = cmesh->getMeshConnectivity().getOtherCell(cface->geometricEntity(), ccell->geometricEntity());
    ComputationalCell::Ptr ccother = cmesh->getMapper().getComputationalCell(cother);

    IGeometricEntity::Id_t ccother_id = ccell->geometricEntity()->meshId() == 3ull ? 4ull : 3ull;
    CPPUNIT_ASSERT_MESSAGE("Wrong neighbor cell to face 8", ccother->geometricEntity()->meshId() == ccother_id);

    ComputationalVariable::Ptr ccother_var = ccother->getComputationalVariable("Temperature");
    double w1 = *fm.getWeight(*cvar);
    double w2 = *fm.getWeight(*ccother_var);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong weight", 1.8605210188381265, std::fabs(w1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Weights expected to be opposite in value", w1, -w2, 1E-10);

    // check that the source term is correct
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong source term value", 0.0, fm.getSourceTerm().value(), 1E-10);




    /* check the flux of internal face 5 */
    it = std::find_if(internal_face_thread.begin(), internal_face_thread.end(), [](ComputationalFace::Ptr const & cface) -> bool {
        Face::Ptr const & face = cface->geometricEntity();
        return face->meshId() == 5ull;
    });

    CPPUNIT_ASSERT_MESSAGE("Face 5 not found", it != internal_face_thread.end());
    cface = *it;

    bc = cface->getBoundaryCondition();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected no boundary conditions for internal face", BoundaryCondition::Ptr(), bc);

    fm = cface->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Two contributions expected to interior face flux", 2u, fm.size());

    // check that the face flux was computed with the correct cell
    ccell = fm.getCell();
    CPPUNIT_ASSERT_MESSAGE("Wrong computational cell used for face flux calculation", ccell->geometricEntity()->meshId() == 2ull || ccell->geometricEntity()->meshId() == 3ull);

    // check the correct computational variable is used in the comp. face molecule
    cvar = ccell->getComputationalVariable("Temperature");
    cother = cmesh->getMeshConnectivity().getOtherCell(cface->geometricEntity(), ccell->geometricEntity());
    ccother = cmesh->getMapper().getComputationalCell(cother);

    ccother_id = ccell->geometricEntity()->meshId() == 2ull ? 3ull : 2ull;
    CPPUNIT_ASSERT_MESSAGE("Wrong neighbor cell to face 5", ccother->geometricEntity()->meshId() == ccother_id);

    ccother_var = ccother->getComputationalVariable("Temperature");
    w1 = *fm.getWeight(*cvar);
    w2 = *fm.getWeight(*ccother_var);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong weight", 1.8605210188381265, std::abs(w1), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Weights expected to be opposite in value", w1, -w2, 1E-10);

    // check that the source term is correct
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong source term value", 0.0, fm.getSourceTerm().value(), 1E-10);
}

namespace {
    ComputationalCell::Ptr getComputationalCell(Thread<ComputationalCell> const & cell_thread, IGeometricEntity::Id_t cell_id) {
        auto it = std::find_if(cell_thread.begin(), cell_thread.end(), [&cell_id](ComputationalCell::Ptr const & ccell) -> bool {
            Cell::Ptr const & cell = ccell->geometricEntity();
            return cell->meshId() == cell_id;
        });

        if (it == cell_thread.end())
            return nullptr;
        return *it;
    }

}

void
VersteegMalalasekeraTest::evaluateCell5ComputationalMoleculeTest() {
    /* Cell 5 has an outer Dirichlet b.c. face */

    ComputationalMeshBuilder builder(mesh_, bc_);

    // Temperature as cell-centered variable, will be solved for
    builder.addComputationalVariable("Temperature", flux_evaluator);
    builder.addEvaluateCellMolecules(cell_evaluator);
    ComputationalMesh::CPtr cmesh(builder.build());


    auto cell_thread = cmesh->getCellThread();


    /* 
     * Check the computational molecule of cell 5.
     * This cell has two internal faces and one
     * boundary face. Hence, it should have three
     * items in its comp. molecule.
     */
    ComputationalCell::Ptr ccell = getComputationalCell(cell_thread, 4ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 5 not found", ccell.get() != nullptr);

    ComputationalMolecule comp_molecule = ccell->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected 3 items in comp. molecule of cell 5", 3u, comp_molecule.size());

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong source term", 1858.5210188381266, comp_molecule.getSourceTerm().value(), 1E-10);

    // get the ComputationalVariable from neighboring cell 4 that we
    // expect to be in the ComputationalMolecule of cell 5.
    // This contribution comes from the internal face 8 that both cells
    // 4 and 5 share.
    ComputationalCell::Ptr ccell_nbr = getComputationalCell(cell_thread, 3ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 4 not found", ccell_nbr.get() != nullptr);
    ComputationalVariable::Ptr cvar_nbr = ccell_nbr->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 4", cvar_nbr.get() != nullptr);

    boost::optional<double> weight_opt = comp_molecule.getWeight(*cvar_nbr);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 4 to cell 5 in cell 4's computational molecule", bool(weight_opt) == true);
    double weight = *weight_opt;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong weight", 1.8605210188381265, weight, 1E-10);


    // get the ComputationalVariables from neighboring cell 6 that we
    // expect to be in the ComputationalMolecule of cell 5.
    // This contribution comes from the internal face 11 that both cells
    // 5 and 6 share.
    ccell_nbr = getComputationalCell(cell_thread, 5ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 6 not found", ccell_nbr.get() != nullptr);
    cvar_nbr = ccell_nbr->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 6", cvar_nbr.get() != nullptr);

    weight_opt = comp_molecule.getWeight(*cvar_nbr);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 6 to cell 5 in cell 5's computational molecule", bool(weight_opt) == true);
    weight = *weight_opt;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong weight", 1.5, weight, 1E-10);


    // The last contribution in cell 5's computational molecule comes
    // from the boundary face 9.
    cvar_nbr = ccell->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 5", cvar_nbr.get() != nullptr);

    weight_opt = comp_molecule.getWeight(*cvar_nbr);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 5 to itself (Dirichlet b.c. face contribution)", bool(weight_opt) == true);
    weight = *weight_opt;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong weight", -7.0815630565143799, weight, 1E-10);
}

void
VersteegMalalasekeraTest::evaluateCell2ComputationalMoleculeTest() {
    /* Cell 2 has an outer von Neumann b.c. face */

    ComputationalMeshBuilder builder(mesh_, bc_);

    // Temperature as cell-centered variable, will be solved for
    builder.addComputationalVariable("Temperature", flux_evaluator);
    builder.addEvaluateCellMolecules(cell_evaluator);
    ComputationalMesh::CPtr cmesh(builder.build());


    auto cell_thread = cmesh->getCellThread();


    /* 
     * Check the computational molecule of cell 2.
     * This cell has two internal faces and one
     * boundary face. Hence, it should have three
     * items in its comp. molecule.
     */
    ComputationalCell::Ptr ccell = getComputationalCell(cell_thread, 1ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 2 not found", ccell.get() != nullptr);

    ComputationalMolecule comp_molecule = ccell->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected 3 items in comp. molecule of cell 2", 3u, comp_molecule.size());

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong source term", -2.0, comp_molecule.getSourceTerm().value(), 1E-10);
    
    // get the ComputationalVariable from neighboring cell 1 that we
    // expect to be in the ComputationalMolecule of cell 2.
    // This contribution comes from the internal face 0 that both cells
    // 1 and 2 share.
    ComputationalCell::Ptr ccell_nbr = getComputationalCell(cell_thread, 0ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 1 not found", ccell_nbr.get() != nullptr);
    ComputationalVariable::Ptr cvar_nbr = ccell_nbr->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 1", cvar_nbr.get() != nullptr);

    boost::optional<double> weight_opt = comp_molecule.getWeight(*cvar_nbr);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 1 to cell 2 in cell 2's computational molecule", bool(weight_opt) == true);
    double weight = *weight_opt;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong weight", 1.8605210188381265, weight, 1E-10);


    // get the ComputationalVariables from neighboring cell 3 that we
    // expect to be in the ComputationalMolecule of cell 2.
    // This contribution comes from the internal face 4 that both cells
    // 2 and 3 share.
    ccell_nbr = getComputationalCell(cell_thread, 2ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 3 not found", ccell_nbr.get() != nullptr);
    cvar_nbr = ccell_nbr->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 3", cvar_nbr.get() != nullptr);

    weight_opt = comp_molecule.getWeight(*cvar_nbr);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 3 to cell 2 in cell 2's computational molecule", bool(weight_opt) == true);
    weight = *weight_opt;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong weight", 1.8605210188381265, weight, 1E-10);


    // The last contribution in cell 2's computational molecule comes
    // from the boundary face 3.
    cvar_nbr = ccell->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 2", cvar_nbr.get() != nullptr);

    weight_opt = comp_molecule.getWeight(*cvar_nbr);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 2 to itself (b.f. contribution) computational molecule", bool(weight_opt) == true);
    weight = *weight_opt;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong weight", -3.7210420376762530, weight, 1E-10);
}

void
VersteegMalalasekeraTest::compMoleculeCell1AndCell5SimilarTest() {
    /* Check that cells 1 and 5 have similar comp. molecules.
     * see book, page 332 (Node 1) and 333 (Node 5).
     */
    ComputationalMeshBuilder builder(mesh_, bc_);

    // Temperature as cell-centered variable, will be solved for
    builder.addComputationalVariable("Temperature", flux_evaluator);
    builder.addEvaluateCellMolecules(cell_evaluator);
    ComputationalMesh::CPtr cmesh(builder.build());


    auto cell_thread = cmesh->getCellThread();


    // get the ComputationalMolecules of both cells
    ComputationalCell::Ptr ccell_1 = getComputationalCell(cell_thread, 0ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 1 not found", ccell_1.get() != nullptr);

    ComputationalMolecule comp_molecule_1 = ccell_1->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected 3 items in comp. molecule of cell 1", 3u, comp_molecule_1.size());


    ComputationalCell::Ptr ccell_5 = getComputationalCell(cell_thread, 4ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 5 not found", ccell_5.get() != nullptr);

    ComputationalMolecule comp_molecule_5 = ccell_5->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected 3 items in comp. molecule of cell 5", 3u, comp_molecule_5.size());


    /* contribution of cell 2 to cell 1 == contribution of cell 4 to cell 5 */
    ComputationalCell::Ptr ccell = getComputationalCell(cell_thread, 1ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 2 not found", ccell.get() != nullptr);
    ComputationalVariable::Ptr cvar = ccell->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 2", cvar.get() != nullptr);
    boost::optional<double> weight_opt = comp_molecule_1.getWeight(*cvar);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 2 to cell 1 in cell 1's computational molecule", bool(weight_opt) == true);
    double weight_1 = *weight_opt;

    ccell = getComputationalCell(cell_thread, 3ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 4 not found", ccell.get() != nullptr);
    cvar = ccell->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 4", cvar.get() != nullptr);
    weight_opt = comp_molecule_5.getWeight(*cvar);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 4 to cell 5 in cell 5's computational molecule", bool(weight_opt) == true);
    double weight_2 = *weight_opt;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Weight error", weight_1, weight_2, 1E-10);



    /* contribution of cell 8 to cell 1 == contribution of cell 6 to cell 5 */
    ccell = getComputationalCell(cell_thread, 7ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 8 not found", ccell.get() != nullptr);
    cvar = ccell->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 8", cvar.get() != nullptr);
    weight_opt = comp_molecule_1.getWeight(*cvar);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 8 to cell 1 in cell 1's computational molecule", bool(weight_opt) == true);
    weight_1 = *weight_opt;

    ccell = getComputationalCell(cell_thread, 5ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 6 not found", ccell.get() != nullptr);
    cvar = ccell->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 6", cvar.get() != nullptr);
    weight_opt = comp_molecule_5.getWeight(*cvar);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 6 to cell 5 in cell 5's computational molecule", bool(weight_opt) == true);
    weight_2 = *weight_opt;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Weight error", weight_1, weight_2, 1E-10);



    /* contribution of cell 1 to cell 1 == contribution of cell 5 to cell 5 */
    cvar = ccell_1->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 1", cvar.get() != nullptr);
    weight_opt = comp_molecule_1.getWeight(*cvar);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 1 to itself in computational molecule", bool(weight_opt) == true);
    weight_1 = *weight_opt;

    cvar = ccell_5->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 5", cvar.get() != nullptr);
    weight_opt = comp_molecule_5.getWeight(*cvar);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 5 to itself in computational molecule", bool(weight_opt) == true);
    weight_2 = *weight_opt;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Weight error", weight_1, weight_2, 1E-10);


    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Source term error", comp_molecule_1.getSourceTerm().value(), comp_molecule_5.getSourceTerm().value(), 1E-10);
}

void
VersteegMalalasekeraTest::compMoleculeCell6AndCell8SimilarTest() {
    /* Check that cells 6 and 8 have similar comp. molecules.
     * see book, page 333 (Node 6 and 8).
     */
    ComputationalMeshBuilder builder(mesh_, bc_);

    // Temperature as cell-centered variable, will be solved for
    builder.addComputationalVariable("Temperature", flux_evaluator);
    builder.addEvaluateCellMolecules(cell_evaluator);
    ComputationalMesh::CPtr cmesh(builder.build());


    auto cell_thread = cmesh->getCellThread();


    // get the ComputationalMolecules of both cells
    ComputationalCell::Ptr ccell_6 = getComputationalCell(cell_thread, 5ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 6 not found", ccell_6.get() != nullptr);

    ComputationalMolecule comp_molecule_6 = ccell_6->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected 3 items in comp. molecule of cell 6", 3u, comp_molecule_6.size());


    ComputationalCell::Ptr ccell_8 = getComputationalCell(cell_thread, 7ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 8 not found", ccell_8.get() != nullptr);

    ComputationalMolecule comp_molecule_8 = ccell_8->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected 3 items in comp. molecule of cell 5", 3u, comp_molecule_8.size());


    /* contribution of cell 9 to cell 8 == contribution of cell 7 to cell 6 */
    ComputationalCell::Ptr ccell = getComputationalCell(cell_thread, 8ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 9 not found", ccell.get() != nullptr);
    ComputationalVariable::Ptr cvar = ccell->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 9", cvar.get() != nullptr);
    boost::optional<double> weight_opt = comp_molecule_8.getWeight(*cvar);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 9 to cell 8 in cell 8's computational molecule", bool(weight_opt) == true);
    double weight_1 = *weight_opt;

    ccell = getComputationalCell(cell_thread, 6ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 7 not found", ccell.get() != nullptr);
    cvar = ccell->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 7", cvar.get() != nullptr);
    weight_opt = comp_molecule_6.getWeight(*cvar);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 7 to cell 6 in cell 6's computational molecule", bool(weight_opt) == true);
    double weight_2 = *weight_opt;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Weight error", weight_1, weight_2, 1E-10);



    /* contribution of cell 1 to cell 8 == contribution of cell 5 to cell 6 */
    ccell = getComputationalCell(cell_thread, 0ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 1 not found", ccell.get() != nullptr);
    cvar = ccell->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 1", cvar.get() != nullptr);
    weight_opt = comp_molecule_8.getWeight(*cvar);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 1 to cell 8 in cell 8's computational molecule", bool(weight_opt) == true);
    weight_1 = *weight_opt;

    ccell = getComputationalCell(cell_thread, 4ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 5 not found", ccell.get() != nullptr);
    cvar = ccell->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 5", cvar.get() != nullptr);
    weight_opt = comp_molecule_6.getWeight(*cvar);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 5 to cell 6 in cell 6's computational molecule", bool(weight_opt) == true);
    weight_2 = *weight_opt;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Weight error", weight_1, weight_2, 1E-10);



    /* contribution of cell 8 to cell 8 == contribution of cell 6 to cell 6 */
    cvar = ccell_8->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 8", cvar.get() != nullptr);
    weight_opt = comp_molecule_8.getWeight(*cvar);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 8 to itself in computational molecule", bool(weight_opt) == true);
    weight_1 = *weight_opt;

    cvar = ccell_6->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 6", cvar.get() != nullptr);
    weight_opt = comp_molecule_6.getWeight(*cvar);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 6 to itself in computational molecule", bool(weight_opt) == true);
    weight_2 = *weight_opt;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Weight error", weight_1, weight_2, 1E-10);


    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Source term error", comp_molecule_8.getSourceTerm().value(), comp_molecule_6.getSourceTerm().value(), 1E-10);
}

void
VersteegMalalasekeraTest::compMoleculeCell7AndCell9SimilarTest() {
    /* Check that cells 7 and 9 have similar comp. molecules.
     * see book, page 333 (Node 7 and 9).
     */
    ComputationalMeshBuilder builder(mesh_, bc_);

    // Temperature as cell-centered variable, will be solved for
    builder.addComputationalVariable("Temperature", flux_evaluator);
    builder.addEvaluateCellMolecules(cell_evaluator);
    ComputationalMesh::CPtr cmesh(builder.build());


    auto cell_thread = cmesh->getCellThread();


    // get the ComputationalMolecules of both cells
    ComputationalCell::Ptr ccell_7 = getComputationalCell(cell_thread, 6ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 7 not found", ccell_7.get() != nullptr);

    ComputationalMolecule comp_molecule_7 = ccell_7->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected 3 items in comp. molecule of cell 7", 3u, comp_molecule_7.size());


    ComputationalCell::Ptr ccell_9 = getComputationalCell(cell_thread, 8ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 9 not found", ccell_9.get() != nullptr);

    ComputationalMolecule comp_molecule_9 = ccell_9->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected 3 items in comp. molecule of cell 9", 3u, comp_molecule_9.size());

    /* contribution of cell 9' to cell 9 == contribution of cell 7' to cell 7 */
    ComputationalCell::Ptr ccell = getComputationalCell(cell_thread, 17ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 9' not found", ccell.get() != nullptr);
    ComputationalVariable::Ptr cvar = ccell->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 9'", cvar.get() != nullptr);
    boost::optional<double> weight_opt = comp_molecule_9.getWeight(*cvar);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 9' to cell 9 in cell 9's computational molecule", bool(weight_opt) == true);
    double weight_1 = *weight_opt;

    ccell = getComputationalCell(cell_thread, 15ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 7' not found", ccell.get() != nullptr);
    cvar = ccell->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 7'", cvar.get() != nullptr);
    weight_opt = comp_molecule_7.getWeight(*cvar);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 7' to cell 7 in cell 7's computational molecule", bool(weight_opt) == true);
    double weight_2 = *weight_opt;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Weight error", weight_1, weight_2, 1E-10);



    /* contribution of cell 8 to cell 9 == contribution of cell 6 to cell 7 */
    ccell = getComputationalCell(cell_thread, 7ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 8 not found", ccell.get() != nullptr);
    cvar = ccell->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 8", cvar.get() != nullptr);
    weight_opt = comp_molecule_9.getWeight(*cvar);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 8 to cell 9 in cell 9's computational molecule", bool(weight_opt) == true);
    weight_1 = *weight_opt;

    ccell = getComputationalCell(cell_thread, 5ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 6 not found", ccell.get() != nullptr);
    cvar = ccell->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 6", cvar.get() != nullptr);
    weight_opt = comp_molecule_7.getWeight(*cvar);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 6 to cell 7 in cell 7's computational molecule", bool(weight_opt) == true);
    weight_2 = *weight_opt;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Weight error", weight_1, weight_2, 1E-10);



    /* contribution of cell 8 to cell 8 == contribution of cell 6 to cell 6 */
    cvar = ccell_9->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 9", cvar.get() != nullptr);
    weight_opt = comp_molecule_9.getWeight(*cvar);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 8 to itself in computational molecule", bool(weight_opt) == true);
    weight_1 = *weight_opt;

    cvar = ccell_7->getComputationalVariable("Temperature");
    CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 7", cvar.get() != nullptr);
    weight_opt = comp_molecule_7.getWeight(*cvar);
    CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 6 to itself in computational molecule", bool(weight_opt) == true);
    weight_2 = *weight_opt;
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Weight error", weight_1, weight_2, 1E-10);


    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Source term error", comp_molecule_9.getSourceTerm().value(), comp_molecule_7.getSourceTerm().value(), 1E-10);
}

void
VersteegMalalasekeraTest::variousRelationsTest() {
    ComputationalMeshBuilder builder(mesh_, bc_);

    // Temperature as cell-centered variable, will be solved for
    builder.addComputationalVariable("Temperature", flux_evaluator);
    builder.addEvaluateCellMolecules(cell_evaluator);
    ComputationalMesh::CPtr cmesh(builder.build());


    auto cell_thread = cmesh->getCellThread();
    
    {
        /* Versteeg, Malalasekera, p 332 and 333
         * 
         * Equ. for nodes 6 and 8
         * 
         * 1. check that the source term is the same
         * 2. check that the coefficient for T6 and T8 is the same
         */

        // get the ComputationalMolecules of both cells
        ComputationalCell::Ptr ccell_6 = getComputationalCell(cell_thread, 5ull);
        CPPUNIT_ASSERT_MESSAGE("Cell 6 not found", ccell_6.get() != nullptr);

        ComputationalMolecule comp_molecule_6 = ccell_6->getComputationalMolecule("Temperature");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected 3 items in comp. molecule of cell 6", 3u, comp_molecule_6.size());


        ComputationalCell::Ptr ccell_8 = getComputationalCell(cell_thread, 7ull);
        CPPUNIT_ASSERT_MESSAGE("Cell 8 not found", ccell_8.get() != nullptr);

        ComputationalMolecule comp_molecule_8 = ccell_8->getComputationalMolecule("Temperature");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected 3 items in comp. molecule of cell 8", 3u, comp_molecule_8.size());


        /* contribution of cell 6 on itself */
        ComputationalVariable::Ptr cvar = ccell_6->getComputationalVariable("Temperature");
        CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 6", cvar.get() != nullptr);
        boost::optional<double> weight_opt = comp_molecule_6.getWeight(*cvar);
        CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 6 to itself in cell 6's computational molecule", bool(weight_opt) == true);
        double weight_1 = *weight_opt;

        // check that the coefficient of the self-contributions has opposite sign
        // than those to neighboring cells
        std::for_each(comp_molecule_6.begin(), comp_molecule_6.end(), [&cvar, weight_1](ComputationalMolecule::ComputationalMolecule_t::value_type const & item){
            if (item.first != cvar->id())
                CPPUNIT_ASSERT_MESSAGE("Wrong sign in comp. molecule of cell 6", item.second * weight_1 < 0);
        });


        cvar = ccell_8->getComputationalVariable("Temperature");
        CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 8", cvar.get() != nullptr);
        weight_opt = comp_molecule_8.getWeight(*cvar);
        CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 8 to itself in cell 8's computational molecule", bool(weight_opt) == true);
        double weight_2 = *weight_opt;
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Weight error", weight_1, weight_2, 1E-10);

        // check that the coefficient of the self-contributions has opposite sign
        // than those to neighboring cells
        std::for_each(comp_molecule_8.begin(), comp_molecule_8.end(), [&cvar, weight_2](ComputationalMolecule::ComputationalMolecule_t::value_type const & item){
            if (item.first != cvar->id())
                CPPUNIT_ASSERT_MESSAGE("Wrong sign in comp. molecule of cell 8", item.second * weight_2 < 0);
        });


        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Source term error", comp_molecule_6.getSourceTerm().value(), comp_molecule_8.getSourceTerm().value(), 1E-10);
    }

    
    {
        /* Versteeg, Malalasekera, p 332 and 333
         * 
         * Equ. for nodes 2 and 4
         * 
         * 1. check that the source term is the same (i.e. = 0)
         * 2. check that the coefficient for T2 and T4 is the same
         */

        // get the ComputationalMolecules of both cells
        ComputationalCell::Ptr ccell_2 = getComputationalCell(cell_thread, 1ull);
        CPPUNIT_ASSERT_MESSAGE("Cell 2 not found", ccell_2.get() != nullptr);

        ComputationalMolecule comp_molecule_2 = ccell_2->getComputationalMolecule("Temperature");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected 3 items in comp. molecule of cell 2", 3u, comp_molecule_2.size());


        ComputationalCell::Ptr ccell_4 = getComputationalCell(cell_thread, 3ull);
        CPPUNIT_ASSERT_MESSAGE("Cell 4 not found", ccell_4.get() != nullptr);

        ComputationalMolecule comp_molecule_4 = ccell_4->getComputationalMolecule("Temperature");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected 3 items in comp. molecule of cell 4", 3u, comp_molecule_4.size());



        /* contribution of cell 2 on itself */
        ComputationalVariable::Ptr cvar = ccell_2->getComputationalVariable("Temperature");
        CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 2", cvar.get() != nullptr);
        boost::optional<double> weight_opt = comp_molecule_2.getWeight(*cvar);
        CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 2 to itself in cell 2's computational molecule", bool(weight_opt) == true);
        double weight_1 = *weight_opt;

        // check that the coefficient of the self-contributions has opposite sign
        // than those to neighboring cells
        std::for_each(comp_molecule_2.begin(), comp_molecule_2.end(), [&cvar, weight_1](ComputationalMolecule::ComputationalMolecule_t::value_type const & item){
            if (item.first != cvar->id())
                CPPUNIT_ASSERT_MESSAGE("Wrong sign in comp. molecule of cell 2", item.second * weight_1 < 0);
        });


        cvar = ccell_4->getComputationalVariable("Temperature");
        CPPUNIT_ASSERT_MESSAGE("Computational variable \"Temperature\" not found in cell 4", cvar.get() != nullptr);
        weight_opt = comp_molecule_4.getWeight(*cvar);
        CPPUNIT_ASSERT_MESSAGE("Expected to find contribution from cell 8 to itself in cell 4's computational molecule", bool(weight_opt) == true);
        double weight_2 = *weight_opt;
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Weight error", weight_1, weight_2, 1E-10);

        // check that the coefficient of the self-contributions has opposite sign
        // than those to neighboring cells
        std::for_each(comp_molecule_4.begin(), comp_molecule_4.end(), [&cvar, weight_2](ComputationalMolecule::ComputationalMolecule_t::value_type const & item){
            if (item.first != cvar->id())
                CPPUNIT_ASSERT_MESSAGE("Wrong sign in comp. molecule of cell 4", item.second * weight_2 < 0);
        });


        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Source term error", comp_molecule_2.getSourceTerm().value(), comp_molecule_4.getSourceTerm().value(), 1E-10);
    }
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
