#include "MeshConnectivityTest.h"

#include "FiniteVolume2DLib/ASCIIMeshReader.h"
#include "FiniteVolume2DLib/Thread.hpp"

#include <boost/filesystem.hpp>

namespace FS = boost::filesystem;


// Static class data members
MeshConnectivityTest::MeshBuilderMock MeshConnectivityTest::mesh_builder_;
MeshConnectivityTest::MeshPtr MeshConnectivityTest::mesh_;


void
MeshConnectivityTest::setUp() {
    mesh_filename_ = "Data\\mesh_connectivity.mesh";

    initMeshConnectivityTest();
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
    Thread<Vertex> const & bvertex_thread = mesh_->getBoundaryVertexThread();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of boundary vertices", 8u, bvertex_thread.size());

    Thread<Vertex> const & ivertex_thread = mesh_->getInteriorVertexThread();
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
    Vertex::Ptr const & v = mesh_builder_.vertex_mgr_->getEntity(6);
    CPPUNIT_ASSERT_MESSAGE("Vertex not found", v != NULL);

    // get mesh connectivity object
    IMeshConnectivity const & mesh_connectivity = mesh_->getMeshConnectivity();

    // get vertex neighbors of 6th vertex
    boost::optional<EntityCollection<Vertex>> vertex_nbr_opt = mesh_connectivity.getVertexNeighbors(v);
    CPPUNIT_ASSERT_MESSAGE("Vertex neighbor error", vertex_nbr_opt);

    EntityCollection<Vertex> const & vertex_nbr = *vertex_nbr_opt;

    // vertices 8, 5, 7
    CPPUNIT_ASSERT_MESSAGE("Vertex 5 expected", vertex_nbr.find(5));
    CPPUNIT_ASSERT_MESSAGE("Vertex 7 expected", vertex_nbr.find(7));
    CPPUNIT_ASSERT_MESSAGE("Vertex 8 expected", vertex_nbr.find(8));
}

void
MeshConnectivityTest::testInternalVertexNeighbors() {
    // get 8th vertex
    Vertex::Ptr const & v = mesh_builder_.vertex_mgr_->getEntity(8);
    CPPUNIT_ASSERT_MESSAGE("Vertex not found", v != NULL);

    // get mesh connectivity object
    IMeshConnectivity const & mesh_connectivity = mesh_->getMeshConnectivity();

    // get vertex neighbors of 6th vertex
    boost::optional<EntityCollection<Vertex>> vertex_nbr_opt = mesh_connectivity.getVertexNeighbors(v);
    CPPUNIT_ASSERT_MESSAGE("Vertex neighbor error", vertex_nbr_opt);

    EntityCollection<Vertex> const & vertex_nbr = *vertex_nbr_opt;

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
MeshConnectivityTest::initMeshConnectivityTest() {
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
