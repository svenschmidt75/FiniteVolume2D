#include "MeshConnectivityTest.h"

#include "FiniteVolume2DLib/ASCIIMeshReader.h"

#include <boost/filesystem.hpp>

namespace FS = boost::filesystem;


// Static class data members
MeshBuilderMock               MeshConnectivityTest::mesh_builder_;
MeshConnectivityTest::MeshPtr MeshConnectivityTest::mesh_;


void
MeshConnectivityTest::setUp() {
    mesh_filename_ = "Data\\mesh_connectivity.mesh";

    initMesh();
}

void
MeshConnectivityTest::tearDown() {
}

void
MeshConnectivityTest::testMeshFileExists() {
    bool exists = FS::exists(mesh_filename_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mesh file not found!", true, exists);
}

void
MeshConnectivityTest::testNumberOfVerices() {
    Thread<Node> const & bvertex_thread = mesh_->getBoundaryNodeThread();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of boundary vertices", 8u, bvertex_thread.size());

    Thread<Node> const & ivertex_thread = mesh_->getInteriorNodeThread();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of internal vertices", 1u, ivertex_thread.size());
}

void
MeshConnectivityTest::testNumberOfFaces() {
    Thread<Face> const & bface_thread = mesh_->getBoundaryFaceThread();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of boundary faces", 8u, bface_thread.size());

    Thread<Face> const & iface_thread = mesh_->getInteriorFaceThread();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of internal faces", 8u, iface_thread.size());
}

void
MeshConnectivityTest::testNumberOfCells() {
    Thread<Cell> const & cell_thread = mesh_->getCellThread();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of cells", 8u, cell_thread.size());
}

void
MeshConnectivityTest::testBoundaryVertexNeighbors() {
    // get 6th vertex
    Node::Ptr const & v = mesh_builder_.node_mgr_->getEntity(6);
    CPPUNIT_ASSERT_MESSAGE("Vertex not found", v != NULL);

    // get mesh connectivity object
    IMeshConnectivity const & mesh_connectivity = mesh_->getMeshConnectivity();

    // get vertex neighbors of 6th vertex
    boost::optional<EntityCollection<Node>> vertex_nbr_opt = mesh_connectivity.getNodeNeighbors(v);
    CPPUNIT_ASSERT_MESSAGE("Vertex neighbor error", vertex_nbr_opt);

    EntityCollection<Node> const & vertex_nbr = *vertex_nbr_opt;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Vertex neighbor size error", 3u, vertex_nbr.size());

    // vertices 8, 5, 7
    CPPUNIT_ASSERT_MESSAGE("Vertex 5 expected", vertex_nbr.find(5));
    CPPUNIT_ASSERT_MESSAGE("Vertex 7 expected", vertex_nbr.find(7));
    CPPUNIT_ASSERT_MESSAGE("Vertex 8 expected", vertex_nbr.find(8));
}

void
MeshConnectivityTest::testInternalVertexNeighbors() {
    // get 8th vertex
    Node::Ptr const & v = mesh_builder_.node_mgr_->getEntity(8);
    CPPUNIT_ASSERT_MESSAGE("Vertex not found", v != NULL);

    // get mesh connectivity object
    IMeshConnectivity const & mesh_connectivity = mesh_->getMeshConnectivity();

    // get vertex neighbors of 8th vertex
    boost::optional<EntityCollection<Node>> vertex_nbr_opt = mesh_connectivity.getNodeNeighbors(v);
    CPPUNIT_ASSERT_MESSAGE("Vertex neighbor error", vertex_nbr_opt);

    EntityCollection<Node> const & vertex_nbr = *vertex_nbr_opt;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Vertex neighbor size error", 8u, vertex_nbr.size());

    // all other vertices
    CPPUNIT_ASSERT_MESSAGE("Vertex 0 expected", vertex_nbr.find(0));
    CPPUNIT_ASSERT_MESSAGE("Vertex 1 expected", vertex_nbr.find(1));
    CPPUNIT_ASSERT_MESSAGE("Vertex 2 expected", vertex_nbr.find(2));
    CPPUNIT_ASSERT_MESSAGE("Vertex 3 expected", vertex_nbr.find(3));
    CPPUNIT_ASSERT_MESSAGE("Vertex 4 expected", vertex_nbr.find(4));
    CPPUNIT_ASSERT_MESSAGE("Vertex 5 expected", vertex_nbr.find(5));
    CPPUNIT_ASSERT_MESSAGE("Vertex 6 expected", vertex_nbr.find(6));
    CPPUNIT_ASSERT_MESSAGE("Vertex 7 expected", vertex_nbr.find(7));
}

void
MeshConnectivityTest::testInternalVertexFaceNeighbors() {
    // get 8th vertex
    Node::Ptr const & v = mesh_builder_.node_mgr_->getEntity(8);
    CPPUNIT_ASSERT_MESSAGE("Vertex not found", v != NULL);

    // get mesh connectivity object
    IMeshConnectivity const & mesh_connectivity = mesh_->getMeshConnectivity();

    // get attached faces to 8th vertex
    boost::optional<EntityCollection<Face>> face_nbrs_opt = mesh_connectivity.getFacesAttachedToNode(v);
    CPPUNIT_ASSERT_MESSAGE("Vertex neighbor error", face_nbrs_opt);

    EntityCollection<Face> const & face_nbrs = *face_nbrs_opt;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Vertex neighbor size error", 8u, face_nbrs.size());

    // all faces attached to vertex 8
    CPPUNIT_ASSERT_MESSAGE("Face 0 expected", face_nbrs.find(0));
    CPPUNIT_ASSERT_MESSAGE("Face 1 expected", face_nbrs.find(1));
    CPPUNIT_ASSERT_MESSAGE("Face 2 expected", face_nbrs.find(2));
    CPPUNIT_ASSERT_MESSAGE("Face 3 expected", face_nbrs.find(3));
    CPPUNIT_ASSERT_MESSAGE("Face 12 expected", face_nbrs.find(12));
    CPPUNIT_ASSERT_MESSAGE("Face 13 expected", face_nbrs.find(13));
    CPPUNIT_ASSERT_MESSAGE("Face 14 expected", face_nbrs.find(14));
    CPPUNIT_ASSERT_MESSAGE("Face 15 expected", face_nbrs.find(15));
}

void
MeshConnectivityTest::testBoundaryVertexFaceNeighbors() {
    // get 7th vertex
    Node::Ptr const & v = mesh_builder_.node_mgr_->getEntity(7);
    CPPUNIT_ASSERT_MESSAGE("Vertex not found", v != NULL);

    // get mesh connectivity object
    IMeshConnectivity const & mesh_connectivity = mesh_->getMeshConnectivity();

    // get attached faces to 7th vertex
    boost::optional<EntityCollection<Face>> face_nbrs_opt = mesh_connectivity.getFacesAttachedToNode(v);
    CPPUNIT_ASSERT_MESSAGE("Vertex neighbor error", face_nbrs_opt);

    EntityCollection<Face> const & face_nbrs = *face_nbrs_opt;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Vertex neighbor size error", 3u, face_nbrs.size());

    // all faces attached to vertex 7
    CPPUNIT_ASSERT_MESSAGE("Face 10 expected", face_nbrs.find(10));
    CPPUNIT_ASSERT_MESSAGE("Face 15 expected", face_nbrs.find(15));
    CPPUNIT_ASSERT_MESSAGE("Face 11 expected", face_nbrs.find(11));
}

void
MeshConnectivityTest::testInternalVertexCellNeighbors() {
    // get 8th vertex
    Node::Ptr const & v = mesh_builder_.node_mgr_->getEntity(8);
    CPPUNIT_ASSERT_MESSAGE("Vertex not found", v != NULL);

    // get mesh connectivity object
    IMeshConnectivity const & mesh_connectivity = mesh_->getMeshConnectivity();

    // get attached cells to 8th vertex
    boost::optional<EntityCollection<Cell>> cell_nbrs_opt = mesh_connectivity.getCellsAttachedToNode(v);
    CPPUNIT_ASSERT_MESSAGE("Vertex neighbor error", cell_nbrs_opt);

    EntityCollection<Cell> const & cell_nbrs = *cell_nbrs_opt;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Vertex neighbor size error", 8u, cell_nbrs.size());

    // all cells attached to vertex 8
    CPPUNIT_ASSERT_MESSAGE("Cell 0 expected", cell_nbrs.find(0));
    CPPUNIT_ASSERT_MESSAGE("Cell 1 expected", cell_nbrs.find(1));
    CPPUNIT_ASSERT_MESSAGE("Cell 2 expected", cell_nbrs.find(2));
    CPPUNIT_ASSERT_MESSAGE("Cell 3 expected", cell_nbrs.find(3));
    CPPUNIT_ASSERT_MESSAGE("Cell 4 expected", cell_nbrs.find(4));
    CPPUNIT_ASSERT_MESSAGE("Cell 5 expected", cell_nbrs.find(5));
    CPPUNIT_ASSERT_MESSAGE("Cell 6 expected", cell_nbrs.find(6));
    CPPUNIT_ASSERT_MESSAGE("Cell 7 expected", cell_nbrs.find(7));
}


void
MeshConnectivityTest::testBoundaryVertexCellNeighbors() {
    // get 3th vertex
    Node::Ptr const & v = mesh_builder_.node_mgr_->getEntity(3);
    CPPUNIT_ASSERT_MESSAGE("Vertex not found", v != NULL);

    // get mesh connectivity object
    IMeshConnectivity const & mesh_connectivity = mesh_->getMeshConnectivity();

    // get attached cells to 3th vertex
    boost::optional<EntityCollection<Cell>> cell_nbrs_opt = mesh_connectivity.getCellsAttachedToNode(v);
    CPPUNIT_ASSERT_MESSAGE("Vertex neighbor error", cell_nbrs_opt);

    EntityCollection<Cell> const & cell_nbrs = *cell_nbrs_opt;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Vertex neighbor size error", 2u, cell_nbrs.size());

    // all cells attached to vertex 3
    CPPUNIT_ASSERT_MESSAGE("Cell 2 expected", cell_nbrs.find(2));
    CPPUNIT_ASSERT_MESSAGE("Cell 3 expected", cell_nbrs.find(3));
}

void
MeshConnectivityTest::testInteriorFaceGetAttachedCells() {
    // get 2nd face
    Face::Ptr const & f = mesh_builder_.face_mgr_->getEntity(2);
    CPPUNIT_ASSERT_MESSAGE("Face not found", f != NULL);

    // get mesh connectivity object
    IMeshConnectivity const & mesh_connectivity = mesh_->getMeshConnectivity();

    // get attached cells to 2nd face
    boost::optional<EntityCollection<Cell>> cells_opt = mesh_connectivity.getCellsAttachedToFace(f);
    CPPUNIT_ASSERT_MESSAGE("Attached cell error", cells_opt);

    EntityCollection<Cell> const & cells = *cells_opt;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cell neighbor size error", 2u, cells.size());

    // all cells attached to face 2
    CPPUNIT_ASSERT_MESSAGE("Cell 3 expected", cells.find(3));
    CPPUNIT_ASSERT_MESSAGE("Cell 4 expected", cells.find(4));
}

void
MeshConnectivityTest::testBoundaryFaceGetAttachedCells() {
    // get 9th face
    Face::Ptr const & f = mesh_builder_.face_mgr_->getEntity(9);
    CPPUNIT_ASSERT_MESSAGE("Face not found", f != NULL);

    // get mesh connectivity object
    IMeshConnectivity const & mesh_connectivity = mesh_->getMeshConnectivity();

    // get attached cell to 9th face
    boost::optional<EntityCollection<Cell>> cells_opt = mesh_connectivity.getCellsAttachedToFace(f);
    CPPUNIT_ASSERT_MESSAGE("Attached cell error", cells_opt);

    EntityCollection<Cell> const & cells = *cells_opt;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cell neighbor size error", 1u, cells.size());

    // cell attached to face 9
    CPPUNIT_ASSERT_MESSAGE("Cell 5 expected", cells.find(5));
}

void
MeshConnectivityTest::testInteriorGetOtherCell() {
    // get 3rd face
    Face::Ptr const & f = mesh_builder_.face_mgr_->getEntity(3);
    CPPUNIT_ASSERT_MESSAGE("Face not found", f != NULL);

    // get 6th cell
    Cell::Ptr const & c = mesh_builder_.cell_mgr_->getEntity(6);
    CPPUNIT_ASSERT_MESSAGE("Cell not found", c != NULL);

    // get mesh connectivity object
    IMeshConnectivity const & mesh_connectivity = mesh_->getMeshConnectivity();

    Cell::Ptr other_cell = mesh_connectivity.getOtherCell(f, c);
    CPPUNIT_ASSERT_MESSAGE("Cell 5 expected", other_cell != NULL);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cell 5 expected", static_cast<IGeometricEntity::Id_t>(5), other_cell->meshId());
}

void
MeshConnectivityTest::testBoundaryGetOtherCell() {
    // get 11th face
    Face::Ptr const & f = mesh_builder_.face_mgr_->getEntity(11);
    CPPUNIT_ASSERT_MESSAGE("Face not found", f != NULL);

    // get 7th cell
    Cell::Ptr const & c = mesh_builder_.cell_mgr_->getEntity(7);
    CPPUNIT_ASSERT_MESSAGE("Cell not found", c != NULL);

    // get mesh connectivity object
    IMeshConnectivity const & mesh_connectivity = mesh_->getMeshConnectivity();

    Cell::Ptr other_cell = mesh_connectivity.getOtherCell(f, c);
    CPPUNIT_ASSERT_MESSAGE("No cell expected", other_cell == NULL);
}

void
MeshConnectivityTest::initMesh() {
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
