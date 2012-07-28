#include "VersteegMalalasekeraMeshDistortedTest.h"

#include "FiniteVolume2DLib/ASCIIMeshReader.h"
#include "FiniteVolume2DLib/Math.h"
#include "FiniteVolume2DLib/MeshChecker.h"
#include "FiniteVolume2DLib/LineSegment.h"
#include "FiniteVolume2DLib/Line.h"
#include "FiniteVolume2DLib/GeometricHelper.h"
#include "FiniteVolume2DLib/Util.h"

#include "FiniteVolume2D/ComputationalMeshBuilder.h"
#include "FiniteVolume2D/ComputationalMeshSolverHelper.h"
#include "FiniteVolume2D/IComputationalGridAccessor.h"

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/any.hpp>

#include <algorithm>
#include <exception>
#include <cassert>
#include <cmath>

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

        // account for r.h.s.
        flux += -2.0 * cmesh.getCellThread().size();

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
        Line cell_centroid_ls(c1_centroid, c2_centroid);
        LineSegment face_ls(cface->startNode().geometricEntity()->location(), cface->endNode().geometricEntity()->location());

        boost::optional<Vertex> ip_opt = GeometricHelper::intersect(face_ls, cell_centroid_ls);

        assert(bool(ip_opt));
        if (!ip_opt) {
            boost::format format = boost::format("cell_skewness: No intersection found between face centroids %1%, %2% and face %3%!\n")
                % c1->geometricEntity()->meshId()
                % c2->geometricEntity()->meshId()
                % cface->geometricEntity()->meshId();

            Util::error(format.str());

            // have to throw because we only return by reference
            throw std::out_of_range(format.str().c_str());
        }

        Vertex intersection_point = *ip_opt;

        // compute face mid point
        Vertex midpoint = (cface->startNode().location() + cface->endNode().location()) / 2.0;

        // compute distance of intersection point from face midpoint
        double dst = Math::dist(midpoint, intersection_point);

        // distance face vertex - face midpoint
        double ref_dst = Math::dist(cface->startNode().geometricEntity()->location(), midpoint);

        double rel_dst = dst / ref_dst;

        std::cout << "Skewness cells " << c1->geometricEntity()->meshId() << ", " << c2->geometricEntity()->meshId()
            << " and face " << cface->geometricEntity()->meshId() << ": " << rel_dst << std::endl;

        // insert value into face for later use
        cface->addUserDefValue("CellSkewness", std::make_pair(c1->geometricEntity()->meshId(), rel_dst));

        return rel_dst;
    }
   
    double
    cell_aspect_ratio(ComputationalCell::Ptr const & c1, ComputationalCell::Ptr const & c2, ComputationalFace::Ptr const & cface) {
        /* Compute the distance from the cell centroid of cell c1
         * to the face mid point. Compare to the distance of both
         * cell centroids. Ideally, the ratio is 1/2.
         */
        if (cface->getEntityType() == IGeometricEntity::BOUNDARY)
            return 0;

        Vertex c1_centroid = c1->centroid();
        Vertex c2_centroid = c2->centroid();

        // compute face mid point
        Vertex midpoint = (cface->startNode().location() + cface->endNode().location()) / 2.0;

        // compute distance of intersection point from face midpoint
        double dst = Math::dist(c1_centroid, midpoint);

        // distance cell centroids
        double ref_dst = Math::dist(c1_centroid, c2_centroid);

        double rel_dst = dst / ref_dst;

        std::cout << "Aspect ratio of cell " << c1->geometricEntity()->meshId() << " with cell " << c2->geometricEntity()->meshId()
            << " and face " << cface->geometricEntity()->meshId() << ": " << rel_dst << std::endl;

        // insert value into face for later use
        cface->addUserDefValue("CellAspectRatio", std::make_pair(c1->geometricEntity()->meshId(), rel_dst));

        return rel_dst;
    }

    double
    normal_deviation(ComputationalCell::Ptr const & c1, ComputationalCell::Ptr const & c2, ComputationalFace::Ptr const & cface) {
        /* Compute the angle between the line segment
         * cell centroid c1 and cell centroid c2 with
         * the face normal.
         * Ideally, the angle should be zero.
         */
        if (cface->getEntityType() == IGeometricEntity::BOUNDARY)
            return 0;

        Vertex c1_centroid = c1->centroid();
        Vertex c2_centroid = c2->centroid();
        Vector cell_centroid_vec = c2_centroid - c1_centroid;
        double cell_centroid_vec_length = cell_centroid_vec.norm();

        // face normal pointing to cell c2
        Vector face_normal = cface->normal();
        double face_normal_length = face_normal.norm();

        double cos_theta = Math::dot(cell_centroid_vec, face_normal) / cell_centroid_vec_length / face_normal_length;

        /* Note: arccos is unique for theta in (0, pi), hence
         * all is fine as log as the projection of vector 1
         * onto vector 2 has a positive component.
         * 
         * Note that if the line segment connecting both cell centroids
         * is NOT aligned with the coordinate axes, we will get a deviation
         * from the face normal direction. This is typical for unstructured
         * meshes, where this kind of numerical issues arise.
         */
        double theta = std::acos(cos_theta);

        std::cout << "Normal deviation of cell " << c1->geometricEntity()->meshId() << " with cell " << c2->geometricEntity()->meshId()
            << " and face " << cface->geometricEntity()->meshId() << ": " << theta << std::endl;


        // insert value into face for later use
        cface->addUserDefValue("NormalDeviation", std::make_pair(c1->geometricEntity()->meshId(), theta));

        return theta;
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

        // compute the cell skewness
        cell_skewness(ccell, cell_nbr, cface);
        cell_aspect_ratio(ccell, cell_nbr, cface);
        normal_deviation(ccell, cell_nbr, cface);

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
VersteegMalalasekeraMeshDistortedTest::cellSkewnessTest() {
    ComputationalMeshBuilder builder(mesh_, bc_);

    // Temperature as cell-centered variable, will be solved for
    builder.addComputationalVariable("Temperature", flux_evaluator);
    builder.addEvaluateCellMolecules(cell_evaluator);
    ComputationalMesh::CPtr cmesh(builder.build());


    ComputationalMeshSolverHelper helper(*cmesh);
    CPPUNIT_ASSERT_MESSAGE("Could not solve for computational mesh", helper.solve());



    auto internal_face_thread = cmesh->getFaceThread(IGeometricEntity::Entity_t::INTERIOR);



    /*
     * Check the cell skewness of internal face 11.
     * Should be unchanged compared to the undistorted mesh.
     */
    auto it = std::find_if(internal_face_thread.begin(), internal_face_thread.end(), [](ComputationalFace::Ptr const & cface) -> bool {
        Face::Ptr const & face = cface->geometricEntity();
        return face->meshId() == 11ull;
    });

    CPPUNIT_ASSERT_MESSAGE("Face 11 not found", it != internal_face_thread.end());
    ComputationalFace::Ptr cface = *it;

    // get user-defined variable CellSkewness
    boost::any cell_skewness_any = cface->getUserDefValue("CellSkewness");

    IGeometricEntity::Id_t cell_id;
    double cs;

    std::tie(cell_id, cs) = boost::any_cast<std::pair<IGeometricEntity::Id_t, double>>(cell_skewness_any);

    // the cell skewness should be 0
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected cell aspect ratio", 0.0, cs, 1E-10);



    /*
     * Check the cell skewness of internal face 8.
     * Compared to the undistorted mesh, the skewness
     * has changed due to the change of vertex F.
     */
    it = std::find_if(internal_face_thread.begin(), internal_face_thread.end(), [](ComputationalFace::Ptr const & cface) -> bool {
        Face::Ptr const & face = cface->geometricEntity();
        return face->meshId() == 8ull;
    });

    CPPUNIT_ASSERT_MESSAGE("Face 8 not found", it != internal_face_thread.end());
    cface = *it;

    // get user-defined variable CellSkewness
    cell_skewness_any = cface->getUserDefValue("CellSkewness");

    std::tie(cell_id, cs) = boost::any_cast<std::pair<IGeometricEntity::Id_t, double>>(cell_skewness_any);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected normal deviation", 0.037037037037037202, cs, 1E-10);




    /*
     * Check the cell skewness of internal face 4.
     * Compared to the undistorted mesh, the skewness
     * has changed due to the change of vertex B.
     */
    it = std::find_if(internal_face_thread.begin(), internal_face_thread.end(), [](ComputationalFace::Ptr const & cface) -> bool {
        Face::Ptr const & face = cface->geometricEntity();
        return face->meshId() == 4ull;
    });

    CPPUNIT_ASSERT_MESSAGE("Face 4 not found", it != internal_face_thread.end());
    cface = *it;

    // get user-defined variable CellSkewness
    cell_skewness_any = cface->getUserDefValue("CellSkewness");

    std::tie(cell_id, cs) = boost::any_cast<std::pair<IGeometricEntity::Id_t, double>>(cell_skewness_any);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected normal deviation", 0.090909090909091092, cs, 1E-10);




    /*
     * Check the cell skewness of internal face 1.
     * Although vertex B was changes, the line segment
     * connecting cells 1 and 8 is coordinate aligned
     * ans still goes through the face midpoint, hence
     * no change compared to the undistorted mesh.
     */
    it = std::find_if(internal_face_thread.begin(), internal_face_thread.end(), [](ComputationalFace::Ptr const & cface) -> bool {
        Face::Ptr const & face = cface->geometricEntity();
        return face->meshId() == 1ull;
    });

    CPPUNIT_ASSERT_MESSAGE("Face 1 not found", it != internal_face_thread.end());
    cface = *it;

    // get user-defined variable CellSkewness
    cell_skewness_any = cface->getUserDefValue("CellSkewness");

    std::tie(cell_id, cs) = boost::any_cast<std::pair<IGeometricEntity::Id_t, double>>(cell_skewness_any);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected normal deviation", 0.0, cs, 1E-10);
}

void
VersteegMalalasekeraMeshDistortedTest::cellAspectRatioTest() {
    ComputationalMeshBuilder builder(mesh_, bc_);

    // Temperature as cell-centered variable, will be solved for
    builder.addComputationalVariable("Temperature", flux_evaluator);
    builder.addEvaluateCellMolecules(cell_evaluator);
    ComputationalMesh::CPtr cmesh(builder.build());


    ComputationalMeshSolverHelper helper(*cmesh);
    CPPUNIT_ASSERT_MESSAGE("Could not solve for computational mesh", helper.solve());



    auto internal_face_thread = cmesh->getFaceThread(IGeometricEntity::Entity_t::INTERIOR);



    /*
     * Check the cell aspect ratio of internal face 4.
     * Because the mesh vertex B has been moved up,
     * the distance from the cell centroid to the face
     * midpoint is slightly larger compared to the
     * undistorted mesh (0.5).
     */
    auto it = std::find_if(internal_face_thread.begin(), internal_face_thread.end(), [](ComputationalFace::Ptr const & cface) -> bool {
        Face::Ptr const & face = cface->geometricEntity();
        return face->meshId() == 4ull;
    });

    CPPUNIT_ASSERT_MESSAGE("Face 4 not found", it != internal_face_thread.end());
    ComputationalFace::Ptr cface = *it;

    // get user-defined variable CellAspectRatio
    boost::any cell_aspect_ratio_any = cface->getUserDefValue("CellAspectRatio");

    IGeometricEntity::Id_t cell_id;
    double car;

    std::tie(cell_id, car) = boost::any_cast<std::pair<IGeometricEntity::Id_t, double>>(cell_aspect_ratio_any);

    // The cell aspect ratio should be 0.5. This means the line segment
    // connecting both cell centroids intersects the face half way.
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected cell aspect ratio", 0.58834840541455224, car, 1E-10);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong cell", 1ull, cell_id);



    /*
     * Check the cell aspect ratio of internal face 10.
     * Although neither face vertex was altered, the mesh vertex F was
     * changed which has an effect on cell centroid 4, hence the slight
     * deviation from 0.5.
     */
    it = std::find_if(internal_face_thread.begin(), internal_face_thread.end(), [](ComputationalFace::Ptr const & cface) -> bool {
        Face::Ptr const & face = cface->geometricEntity();
        return face->meshId() == 10ull;
    });

    CPPUNIT_ASSERT_MESSAGE("Face 10 not found", it != internal_face_thread.end());
    cface = *it;

    // get user-defined variable CellAspectRatio
    cell_aspect_ratio_any = cface->getUserDefValue("CellAspectRatio");

    std::tie(cell_id, car) = boost::any_cast<std::pair<IGeometricEntity::Id_t, double>>(cell_aspect_ratio_any);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected normal deviation", 0.51140831195675873, car, 1E-10);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong cell", 4ull, cell_id);
}

void
VersteegMalalasekeraMeshDistortedTest::normalDeviationTest() {
    ComputationalMeshBuilder builder(mesh_, bc_);

    // Temperature as cell-centered variable, will be solved for
    builder.addComputationalVariable("Temperature", flux_evaluator);
    builder.addEvaluateCellMolecules(cell_evaluator);
    ComputationalMesh::CPtr cmesh(builder.build());


    ComputationalMeshSolverHelper helper(*cmesh);
    CPPUNIT_ASSERT_MESSAGE("Could not solve for computational mesh", helper.solve());



    auto internal_face_thread = cmesh->getFaceThread(IGeometricEntity::Entity_t::INTERIOR);



    /*
     * Check the normal deviation of internal face 0.
     * Vertex B was moved up, hence the face normal
     * of face 0 is quite a deviation from the
     * line segment connecting cell centroids 1 and 2.
     */
    auto it = std::find_if(internal_face_thread.begin(), internal_face_thread.end(), [](ComputationalFace::Ptr const & cface) -> bool {
        Face::Ptr const & face = cface->geometricEntity();
        return face->meshId() == 0ull;
    });

    CPPUNIT_ASSERT_MESSAGE("Face 0 not found", it != internal_face_thread.end());
    ComputationalFace::Ptr cface = *it;

    // get user-defined variable NormalDeviation
    boost::any normal_deviation_any = cface->getUserDefValue("NormalDeviation");

    IGeometricEntity::Id_t cell_id;
    double nd;

    std::tie(cell_id, nd) = boost::any_cast<std::pair<IGeometricEntity::Id_t, double>>(normal_deviation_any);

    // the line (cell centroid cell_id, face midpoint) is not aligned with
    // the coordinate axis, hence the normal deviation > 0.
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected normal deviation", 0.26625204915092515, nd, 1E-10);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong cell", 0ull, cell_id);



    /*
     * Check the normal deviation of internal face 1.
     * Vertex B was moved up, hence the face normal
     * of face 1 is quite a deviation from the
     * line segment connecting cell centroids 1 and 8.
     */
    it = std::find_if(internal_face_thread.begin(), internal_face_thread.end(), [](ComputationalFace::Ptr const & cface) -> bool {
        Face::Ptr const & face = cface->geometricEntity();
        return face->meshId() == 1ull;
    });

    CPPUNIT_ASSERT_MESSAGE("Face 1 not found", it != internal_face_thread.end());
    cface = *it;

    // get user-defined variable NormalDeviation
    normal_deviation_any = cface->getUserDefValue("NormalDeviation");

    std::tie(cell_id, nd) = boost::any_cast<std::pair<IGeometricEntity::Id_t, double>>(normal_deviation_any);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected normal deviation", 0.46364760900080615, nd, 1E-10);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong cell", 0ull, cell_id);



    /*
     * Check the normal deviation of internal face 8.
     * Vertex F was moved to the right, hence the face normal
     * of face 8 is quite a deviation from the
     * line segment connecting cell centroids 4 and 5.
     */
    it = std::find_if(internal_face_thread.begin(), internal_face_thread.end(), [](ComputationalFace::Ptr const & cface) -> bool {
        Face::Ptr const & face = cface->geometricEntity();
        return face->meshId() == 8ull;
    });

    CPPUNIT_ASSERT_MESSAGE("Face 8 not found", it != internal_face_thread.end());
    cface = *it;

    // get user-defined variable NormalDeviation
    normal_deviation_any = cface->getUserDefValue("NormalDeviation");

    std::tie(cell_id, nd) = boost::any_cast<std::pair<IGeometricEntity::Id_t, double>>(normal_deviation_any);

    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Unexpected normal deviation", 0.055498505245717616, nd, 1E-10);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong cell", 3ull, cell_id);
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
