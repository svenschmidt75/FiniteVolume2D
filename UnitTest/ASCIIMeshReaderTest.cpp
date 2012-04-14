#include "ASCIIMeshReaderTest.h"

#include "FiniteVolume2DLib/ASCIIMeshReader.h"

#include <boost/filesystem.hpp>

namespace FS = boost::filesystem;


// Static class data members
ASCIIMeshReaderTest::MockMeshBuilder ASCIIMeshReaderTest::mock_builder_;


void
ASCIIMeshReaderTest::setUp() {
    mesh_filename_ = "Data\\grid.mesh";

    initMesh();
}

void
ASCIIMeshReaderTest::tearDown() {
}

void
ASCIIMeshReaderTest::testMeshFileExists() {
    bool exists = FS::exists(mesh_filename_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mesh file not found!", true, exists);
}

void
ASCIIMeshReaderTest::testNumberOfVertices() {
    int expect = mock_builder_.nodes_.size();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of vertices", 6, expect);
}

void
ASCIIMeshReaderTest::testVertex1() {
    MockMeshBuilder::NodeStr const & vertex_str = mock_builder_.nodes_[2];

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected boundary vertex", IGeometricEntity::BOUNDARY, vertex_str.entity_type_);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong vertex x coordinate", 2.123432, vertex_str.x_, 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong vertex y coordinate", -0.3847, vertex_str.y_, 1E-10);
}

void
ASCIIMeshReaderTest::testVertex2() {
    MockMeshBuilder::NodeStr const & vertex_str = mock_builder_.nodes_[5];

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Unexpected boundary vertex", IGeometricEntity::INTERIOR, vertex_str.entity_type_);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong vertex x coordinate", 5.123432, vertex_str.x_, 1E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Wrong vertex y coordinate", -0.3847, vertex_str.y_, 1E-10);
}

void
ASCIIMeshReaderTest::testNumberOfFaces() {
    int expect = mock_builder_.faces_.size();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of faces", 4, expect);
}

void
ASCIIMeshReaderTest::testFace() {
    MockMeshBuilder::FaceStr const & face_str = mock_builder_.faces_[1];

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of vertices", 2u, face_str.node_ids_.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Expected boundary face", IGeometricEntity::BOUNDARY, face_str.entity_type_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong face vertex id", static_cast<IGeometricEntity::Id_t>(0), face_str.node_ids_[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong face vertex id", static_cast<IGeometricEntity::Id_t>(2), face_str.node_ids_[1]);
}

void
ASCIIMeshReaderTest::testNumberOfCells() {
    int expect = mock_builder_.cells_.size();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of cells", 1, expect);
}

void
ASCIIMeshReaderTest::testCell() {
    auto const & cell_ids = mock_builder_.cells_[0];

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of faces", 4u, cell_ids.size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong cell face id", static_cast<IGeometricEntity::Id_t>(0), cell_ids[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong cell face id", static_cast<IGeometricEntity::Id_t>(1), cell_ids[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong cell face id", static_cast<IGeometricEntity::Id_t>(2), cell_ids[2]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong cell face id", static_cast<IGeometricEntity::Id_t>(3), cell_ids[3]);
}

void
ASCIIMeshReaderTest::initMesh() {
    static bool init = false;
    if (!init)
    {
        init = true;
        ASCIIMeshReader reader(mesh_filename_, mock_builder_);
        CPPUNIT_ASSERT_MESSAGE("Failed to read mesh file!", reader.read());
    }
}
