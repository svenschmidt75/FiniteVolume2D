#include "EntityTest.h"

#include "FiniteVolume2DLib/FaceManager.h"
#include "FiniteVolume2DLib/NodeManager.h"
#include "FiniteVolume2DLib/ASCIIMeshReader.h"
#include "FiniteVolume2DLib/Face.h"
#include "FiniteVolume2DLib/EntityCollection.hpp"
#include "FiniteVolume2DLib/Vector.h"


// Static class data members
MeshBuilderMock   EntityTest::mesh_builder_;
EntityTest::MeshPtr EntityTest::mesh_;


void
EntityTest::setUp() {
    mesh_filename_ = "Data\\mesh_connectivity.mesh";

    initMesh();
}

void
EntityTest::tearDown() {
}

void
EntityTest::testInternalGetFaceVertices() {
    // get 14th face
    Face::Ptr const & f = mesh_builder_.face_mgr_->getEntity(14);
    CPPUNIT_ASSERT_MESSAGE("Face not found", f != NULL);

    // get attached vertices to 14th face
    EntityCollection<Node> const & vertices = f->getNodes();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Face vertex size error", 2u, vertices.size());

    // all vertices attached to face 14
    CPPUNIT_ASSERT_MESSAGE("Vertex 8 expected", vertices.find(8));
    CPPUNIT_ASSERT_MESSAGE("Vertex 5 expected", vertices.find(5));
}


void
EntityTest::testBoundaryGetFaceVertices() {
    // get 4th face
    Face::Ptr const & f = mesh_builder_.face_mgr_->getEntity(4);
    CPPUNIT_ASSERT_MESSAGE("Face not found", f != NULL);

    // get attached vertices to 4th face
    EntityCollection<Node> const & vertices = f->getNodes();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Face vertex size error", 2u, vertices.size());

    // all vertices attached to face 4
    CPPUNIT_ASSERT_MESSAGE("Vertex 0 expected", vertices.find(0));
    CPPUNIT_ASSERT_MESSAGE("Vertex 1 expected", vertices.find(1));
}

void
EntityTest::testGetCellFaces() {
    // get 4th cell
    Cell::Ptr const & c = mesh_builder_.cell_mgr_->getEntity(4);
    CPPUNIT_ASSERT_MESSAGE("Cell not found", c != NULL);

    // get attached faces to 4th cell
    EntityCollection<Face> const & faces = c->getFaces();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cell face size error", 3u, faces.size());

    // all faces attached to cell 4
    CPPUNIT_ASSERT_MESSAGE("Face 2 expected", faces.find(2));
    CPPUNIT_ASSERT_MESSAGE("Face 8 expected", faces.find(8));
    CPPUNIT_ASSERT_MESSAGE("Face 14 expected", faces.find(14));
}

void
EntityTest::testGetCellVertices() {
    // get 4th cell
    Cell::Ptr const & c = mesh_builder_.cell_mgr_->getEntity(4);
    CPPUNIT_ASSERT_MESSAGE("Cell not found", c != NULL);

    // get attached vertices to 4th cell
    EntityCollection<Node> const & vertices = c->getNodes();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cell vertex size error", 3u, vertices.size());

    // all vertices attached to cell 4
    CPPUNIT_ASSERT_MESSAGE("Vertex 4 expected", vertices.find(4));
    CPPUNIT_ASSERT_MESSAGE("Vertex 5 expected", vertices.find(5));
    CPPUNIT_ASSERT_MESSAGE("Vertex 8 expected", vertices.find(8));
}

void
EntityTest::testFaceArea() {
    // get 13th face
    Face::Ptr f = mesh_builder_.face_mgr_->getEntity(13);
    CPPUNIT_ASSERT_MESSAGE("Face not found", f != NULL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Face area mismatch", sqrt(2.0), f->area(), 1E-10);

    f = mesh_builder_.face_mgr_->getEntity(1);
    CPPUNIT_ASSERT_MESSAGE("Face not found", f != NULL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Face area mismatch", 1.0, f->area(), 1E-10);
}

void
EntityTest::testCellVolume() {
    // get 6th cell
    Cell::Ptr c = mesh_builder_.cell_mgr_->getEntity(6);
    CPPUNIT_ASSERT_MESSAGE("Cell not found", c != NULL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Cell area mismatch", 0.5, c->volume(), 1E-10);

    c = mesh_builder_.cell_mgr_->getEntity(2);
    CPPUNIT_ASSERT_MESSAGE("Cell not found", c != NULL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Cell area mismatch", 0.5, c->volume(), 1E-10);
}

void
EntityTest::testFaceCentroid() {
    // get 14th face
    Face::Ptr const & f14 = mesh_builder_.face_mgr_->getEntity(14);
    CPPUNIT_ASSERT_MESSAGE("Face not found", f14 != NULL);

    Vertex face_centroid = f14->centroid();
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Face centroid x mismatch", 0.5, face_centroid.x(), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Face centroid y mismatch", -0.5, face_centroid.y(), 1E-10);


    // get 3rd face
    Face::Ptr const & f3 = mesh_builder_.face_mgr_->getEntity(3);
    CPPUNIT_ASSERT_MESSAGE("Face not found", f3 != NULL);

    face_centroid = f3->centroid();
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Face centroid x mismatch", 0.0, face_centroid.x(), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Face centroid y mismatch", -0.5, face_centroid.y(), 1E-10);
}

void
EntityTest::testCellCentroid() {
    // get 3rd cell
    Cell::Ptr c3 = mesh_builder_.cell_mgr_->getEntity(3);
    CPPUNIT_ASSERT_MESSAGE("Cell not found", c3 != NULL);

    Vertex cell_centroid = c3->centroid();
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Cell centroid x mismatch", 2.0 / 3.0, cell_centroid.x(), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Cell centroid y mismatch", 1.0 / 3.0, cell_centroid.y(), 1E-10);


    // get 6th cell
    Cell::Ptr c6 = mesh_builder_.cell_mgr_->getEntity(6);
    CPPUNIT_ASSERT_MESSAGE("Cell not found", c6 != NULL);

    cell_centroid = c6->centroid();
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Cell centroid x mismatch", -1.0 / 3.0, cell_centroid.x(), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Cell centroid y mismatch", -2.0 / 3.0, cell_centroid.y(), 1E-10);
}

void
EntityTest::testFaceNormal() {
    /* The face normal is different for both attaching cells.
     * Thus, the face normal depends on the cell it is requested
     * for.
     */

    // get 0thd cell
    Cell::Ptr c0 = mesh_builder_.cell_mgr_->getEntity(0);
    CPPUNIT_ASSERT_MESSAGE("Cell not found", c0 != NULL);

    // get 12th face
    Face::Ptr const & f12 = mesh_builder_.face_mgr_->getEntity(12);
    CPPUNIT_ASSERT_MESSAGE("Face not found", f12 != NULL);

    Vector face_normal_c0 = c0->faceNormal(f12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Cell centroid x mismatch", 1.0, face_normal_c0.x(), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Cell centroid y mismatch", 1.0, face_normal_c0.y(), 1E-10);


    // get mesh connectivity object
    IMeshConnectivity const & mesh_connectivity = mesh_->getMeshConnectivity();

    // 1st cell
    Cell::Ptr c1 = mesh_connectivity.getOtherCell(f12, c0);
    CPPUNIT_ASSERT_MESSAGE("Cell not found", c1 != NULL);

    Vector face_normal_c1 = c1->faceNormal(f12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Cell centroid x mismatch", -face_normal_c0.x(), face_normal_c1.x(), 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Cell centroid y mismatch", -face_normal_c0.y(), face_normal_c1.y(), 1E-10);
}

void
EntityTest::initMesh() {
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
    }
}
