#include "MeshConnectivityTest.h"

#include "FiniteVolume2DLib/ASCIIMeshReader.h"

#include <boost/filesystem.hpp>

namespace FS = boost::filesystem;


// Static class data members
MeshConnectivityTest::MeshBuilderMock MeshConnectivityTest::mesh_builder_;
MeshConnectivityTest::MeshPtr MeshConnectivityTest::mesh_;


void
MeshConnectivityTest::setUp() {
    mesh_filename_ = "Data\\grid.mesh";

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
