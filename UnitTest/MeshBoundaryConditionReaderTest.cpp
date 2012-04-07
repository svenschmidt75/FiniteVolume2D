#include "MeshBoundaryConditionReaderTest.h"

#include "FiniteVolume2DLib/ASCIIMeshReader.h"

#include <boost/filesystem.hpp>

namespace FS = boost::filesystem;


// Static class data members
MeshBuilderMock                          MeshBoundaryConditionReaderTest::mesh_builder_;
MeshBoundaryConditionReaderTest::MeshPtr MeshBoundaryConditionReaderTest::mesh_;


void
MeshBoundaryConditionReaderTest::setUp() {
    mesh_filename_ = "Data\\boundary_conditions.mesh";

    initMesh();
}

void
MeshBoundaryConditionReaderTest::tearDown() {
}

void
MeshBoundaryConditionReaderTest::testMeshFileExists() {
    bool exists = FS::exists(mesh_filename_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mesh file not found!", true, exists);
}

void
MeshBoundaryConditionReaderTest::initMesh() {
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
