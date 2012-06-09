#include "MeshCheckerTest.h"

#include "FiniteVolume2DLib/MeshChecker.h"
#include "FiniteVolume2DLib/ASCIIMeshReader.h"

#include <boost/filesystem.hpp>

namespace FS = boost::filesystem;


void
MeshCheckerTest::setUp() {
}

void
MeshCheckerTest::tearDown() {
}

void
MeshCheckerTest::testMissingBoundaryCondition() {
    std::string mesh_filename("Data\\MeshCheck_1.mesh");

    ASCIIMeshReader reader(mesh_filename, mock_builder_);
    CPPUNIT_ASSERT_MESSAGE("Failed to read mesh file!", reader.read());

    bool success = MeshChecker::checkMesh(*mock_builder_.getMesh(), reader.getBoundaryConditions());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mesh check must fail!", false, success);
}

void
MeshCheckerTest::testWrongBoundaryCondition() {
    std::string mesh_filename("Data\\MeshCheck_2.mesh");

    ASCIIMeshReader reader(mesh_filename, mock_builder_);
    CPPUNIT_ASSERT_MESSAGE("Failed to read mesh file!", reader.read());

    bool success = MeshChecker::checkMesh(*mock_builder_.getMesh(), reader.getBoundaryConditions());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mesh check must fail!", false, success);
}

void
MeshCheckerTest::testFaceNeighbors() {
    std::string mesh_filename("Data\\MeshCheck_3.mesh");

    ASCIIMeshReader reader(mesh_filename, mock_builder_);
    CPPUNIT_ASSERT_MESSAGE("Failed to read mesh file!", reader.read());

    bool success = MeshChecker::checkMesh(*mock_builder_.getMesh(), reader.getBoundaryConditions());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mesh check must fail!", false, success);
}
