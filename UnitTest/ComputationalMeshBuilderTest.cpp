#include "ComputationalMeshBuilderTest.h"

#include "FiniteVolume2DLib/ASCIIMeshReader.h"

#include <boost/filesystem.hpp>

namespace FS = boost::filesystem;


// Static class data members
MeshBuilderMock                       ComputationalMeshBuilderTest::mesh_builder_;
ComputationalMeshBuilderTest::MeshPtr ComputationalMeshBuilderTest::mesh_;


void
ComputationalMeshBuilderTest::setUp() {
    mesh_filename_ = "Data\\square.mesh";

    initMesh();
}

void
ComputationalMeshBuilderTest::tearDown() {
}

void
ComputationalMeshBuilderTest::testMeshFileExists() {
    bool exists = FS::exists(mesh_filename_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mesh file not found!", true, exists);
}

// void
// ComputationalMeshBuilderTest::buildMeshTest() {
//     
//     node_mgr_ = NodeManager::create();
//     face_mgr_ = FaceManager::create();
//     cell_mgr_ = CellManager::create();
//     EntityCreatorManager::Ptr emgr = EntityCreatorManager::create(node_mgr_, face_mgr_, cell_mgr_);
//     
//     
//     mesh_builder_ = std::shared_ptr<MeshBuilder>(new MeshBuilder(emgr, bc_));
// 
//     ComputationalMeshBuilder(Mesh::Ptr const & mesh, BoundaryConditionCollection const & bc);
// 
// }







void
ComputationalMeshBuilderTest::initMesh() {
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
