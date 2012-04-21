#include "MeshBoundaryConditionReaderTest.h"

#include "FiniteVolume2DLib/ASCIIMeshReader.h"

#include <boost/filesystem.hpp>

namespace FS = boost::filesystem;


// Static class data members
MeshBuilderMock             MeshBoundaryConditionReaderTest::mock_builder_;
BoundaryConditionCollection MeshBoundaryConditionReaderTest::bc_;


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

    std::for_each(bc_.data_.begin(), bc_.data_.end(), [&dcnt](BoundaryConditionCollection::BoundaryCondition_t::value_type const & in) {
        if (std::get<0>(in.second) == BoundaryConditionCollection::DIRICHLET)
            dcnt++;
    });

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of Dirichlet boundary conditions", 5u, dcnt);
}

void
MeshBoundaryConditionReaderTest::testDirichlet() {
    auto it = bc_.find(9);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of Dirichlet boundary conditions", BoundaryConditionCollection::DIRICHLET, std::get<0>(*it));
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Face area mismatch", -0.938475, std::get<1>(*it), 1E-10);
}

void
MeshBoundaryConditionReaderTest::testNeumann() {
    auto it = bc_.find(8);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of Dirichlet boundary conditions", BoundaryConditionCollection::NEUMANN, std::get<0>(*it));
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Face area mismatch", -4.234, std::get<1>(*it), 1E-10);
}

void
MeshBoundaryConditionReaderTest::testNumberOfNeumannBoundaryConditions() {
    unsigned int dcnt = 0;

    std::for_each(bc_.data_.begin(), bc_.data_.end(), [&dcnt](BoundaryConditionCollection::BoundaryCondition_t::value_type const & in) {
        if (std::get<0>(in.second) == BoundaryConditionCollection::NEUMANN)
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
        bc_ = reader.getBoundaryConditions();
    }
}
