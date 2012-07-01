#include "ComputationalMeshSolverHelperTest.h"

#include "FiniteVolume2D/ComputationalMeshSolverHelper.h"

#include "FiniteVolume2DLib/ASCIIMeshReader.h"
#include "FiniteVolume2DLib/Math.h"
#include "FiniteVolume2DLib/MeshChecker.h"

#include "FiniteVolume2D/ComputationalMeshBuilder.h"
#include "FiniteVolume2D/IComputationalGridAccessor.h"

#include <boost/filesystem.hpp>

#include <algorithm>

#include <cmath>


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
            ComputationalVariable::Ptr const & cvar = ccell->getComputationalVariable(cvar_name);
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

                // insert with opposite sign (convention)
                flux_molecule.add(*cvar, cface->area() / dist);
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

        // insert with opposite sign (convention)
        flux_molecule.add(*cvar,      weight);
        flux_molecule.add(*cvar_nbr, -weight);

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
        st  += 0.0;

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
    helper.setupMatrix();

    IMatrix2D const & m = helper.getMatrix();

    double const a = 7.0815630565143799;
    double const b = 1.8605210188381265;
    double const c = 6.7210420376762539;
    double const d = 3.7210420376762530;
    double const e = -1.5;
    double const zero = 0;

    double value = m(0, 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected coeff. in sparse matrix", a, value, 1E-10);

    value = m(0, 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected coeff. in sparse matrix", -b, value, 1E-10);

    value = m(0, 3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected coeff. in sparse matrix", zero, value, 1E-10);

    value = m(0, 7);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected coeff. in sparse matrix", e, value, 1E-10);

    value = m(1, 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected coeff. in sparse matrix", -b, value, 1E-10);

    value = m(1, 7);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected coeff. in sparse matrix", zero, value, 1E-10);

    value = m(2, 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected coeff. in sparse matrix", c, value, 1E-10);

    value = m(3, 3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected coeff. in sparse matrix", d, value, 1E-10);

    value = m(3, 4);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected coeff. in sparse matrix", -b, value, 1E-10);

    value = m(7, 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected coeff. in sparse matrix", e, value, 1E-10);

    // test a few value against expected values
//    m.print();
}

void
ComputationalMeshSolverHelperTest::rhsTest() {
    ComputationalMeshBuilder builder(mesh_, bc_);

    // Temperature as cell-centered variable, will be solved for
    builder.addComputationalVariable("Temperature", flux_evaluator);
    builder.addEvaluateCellMolecules(cell_evaluator);
    ComputationalMesh::CPtr cmesh(builder.build());


    ComputationalMeshSolverHelper helper(*cmesh);
    helper.solve();

    LinearSolver::RHS_t const & rhs = helper.getRHS();

    double const a = 1860.5210188381266;
    double const b = 1200.0;
    double const c = 744.20840753525090;
    double const zero = 0;

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected coeff. in r.h.s.", rhs[0], a, 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected coeff. in r.h.s.", rhs[1], zero, 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected coeff. in r.h.s.", rhs[2], b, 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected coeff. in r.h.s.", rhs[3], zero, 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected coeff. in r.h.s.", rhs[4], a, 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected coeff. in r.h.s.", rhs[5], c, 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected coeff. in r.h.s.", rhs[6], a, 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected coeff. in r.h.s.", rhs[7], c, 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected coeff. in r.h.s.", rhs[8], a, 1E-10);
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
ComputationalMeshSolverHelperTest::solutionInMeshTest() {
    ComputationalMeshBuilder builder(mesh_, bc_);

    // Temperature as cell-centered variable, will be solved for
    builder.addComputationalVariable("Temperature", flux_evaluator);
    builder.addEvaluateCellMolecules(cell_evaluator);
    ComputationalMesh::CPtr cmesh(builder.build());


    ComputationalMeshSolverHelper helper(*cmesh);
    helper.solve();


    // check solution value in ComputationalMolecules
    double T1_T5 = 441.88218927804616;
    double T2_T4 = 428.95697330354824;
    double T3    = 416.03175732905044;
    double T6_T8 = 313.74140205102862;
    double T7_T9 = 437.91380068367619;


    auto cell_thread = cmesh->getCellThread();



    ComputationalCell::Ptr ccell = getComputationalCell(cell_thread, 0ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 1 not found", ccell.get() != nullptr);
    ComputationalMolecule comp_molecule = ccell->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong solution value", T1_T5, comp_molecule.getValue(), 1E-10);

    ccell = getComputationalCell(cell_thread, 4ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 5 not found", ccell.get() != nullptr);
    comp_molecule = ccell->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong solution value", T1_T5, comp_molecule.getValue(), 1E-10);





    ccell = getComputationalCell(cell_thread, 1ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 2 not found", ccell.get() != nullptr);
    comp_molecule = ccell->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong solution value", T2_T4, comp_molecule.getValue(), 1E-10);

    ccell = getComputationalCell(cell_thread, 3ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 4 not found", ccell.get() != nullptr);
    comp_molecule = ccell->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong solution value", T2_T4, comp_molecule.getValue(), 1E-10);



    

    ccell = getComputationalCell(cell_thread, 2ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 3 not found", ccell.get() != nullptr);
    comp_molecule = ccell->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong solution value", T3, comp_molecule.getValue(), 1E-10);





    ccell = getComputationalCell(cell_thread, 5ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 6 not found", ccell.get() != nullptr);
    comp_molecule = ccell->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong solution value", T6_T8, comp_molecule.getValue(), 1E-10);

    ccell = getComputationalCell(cell_thread, 7ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 8 not found", ccell.get() != nullptr);
    comp_molecule = ccell->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong solution value", T6_T8, comp_molecule.getValue(), 1E-10);





    ccell = getComputationalCell(cell_thread, 6ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 7 not found", ccell.get() != nullptr);
    comp_molecule = ccell->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong solution value", T7_T9, comp_molecule.getValue(), 1E-10);

    ccell = getComputationalCell(cell_thread, 8ull);
    CPPUNIT_ASSERT_MESSAGE("Cell 9 not found", ccell.get() != nullptr);
    comp_molecule = ccell->getComputationalMolecule("Temperature");
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong solution value", T7_T9, comp_molecule.getValue(), 1E-10);
}


void
ComputationalMeshSolverHelperTest::checkFluxBalanceTest() {
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
