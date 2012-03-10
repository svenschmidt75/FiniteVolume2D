#include "MeshConnectivityTest.h"

#include <boost/filesystem.hpp>

namespace FS = boost::filesystem;


// Static class data members
MeshConnectivityTest::MockMeshBuilder MeshConnectivityTest::mock_builder_;


void
MeshConnectivityTest::setUp() {
    mesh_filename_ = "Data\\grid.mesh";

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
