#include "MeshBoundaryConditionReaderTest.h"

#include "FiniteVolume2DLib/ASCIIMeshReader.h"

#include <boost/filesystem.hpp>

namespace FS = boost::filesystem;


// Static class data members
MeshBoundaryConditionReaderTest::MockMeshBuilder MeshBoundaryConditionReaderTest::mock_builder_;


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
MeshBoundaryConditionReaderTest::testNumberOfDirichletBoundaryConditions() {
    unsigned int dcnt = 0;

    std::for_each(mock_builder_.bcs_.begin(), mock_builder_.bcs_.end(), [&dcnt](MockMeshBuilder::BCMap_t::value_type const & in) {
        if (in.second.bc_type_ == BoundaryConditionCollection::DIRICHLET)
            dcnt++;
    });

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of Dirichlet boundary conditions", 5u, dcnt);
}

void
MeshBoundaryConditionReaderTest::testDirichlet() {
    auto it = mock_builder_.bcs_.find(9);
    MockMeshBuilder::BCStr const & p = it->second;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of Dirichlet boundary conditions", BoundaryConditionCollection::DIRICHLET, p.bc_type_);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Face area mismatch", -0.938475, p.bc_value_, 1E-10);
}

void
MeshBoundaryConditionReaderTest::testNeumann() {
    auto it = mock_builder_.bcs_.find(8);
    MockMeshBuilder::BCStr const & p = it->second;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of Dirichlet boundary conditions", BoundaryConditionCollection::NEUMANN, p.bc_type_);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Face area mismatch", -4.234, p.bc_value_, 1E-10);
}

void
MeshBoundaryConditionReaderTest::testNumberOfNeumannBoundaryConditions() {
    unsigned int dcnt = 0;

    std::for_each(mock_builder_.bcs_.begin(), mock_builder_.bcs_.end(), [&dcnt](MockMeshBuilder::BCMap_t::value_type const & in) {
        if (in.second.bc_type_ == BoundaryConditionCollection::NEUMANN)
            dcnt++;
    });

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of von Neumann boundary conditions", 3u, dcnt);
}

void
MeshBoundaryConditionReaderTest::initMesh() {
    static bool init = false;
    if (!init)
    {
        init = true;
        ASCIIMeshReader reader(mesh_filename_, mock_builder_);
        CPPUNIT_ASSERT_MESSAGE("Failed to read mesh file!", reader.read());
    }
}
