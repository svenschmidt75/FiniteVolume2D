#include "ComputationalMeshBuilderTest.h"

#include "FiniteVolume2DLib/ASCIIMeshReader.h"

#include "FiniteVolume2D/ComputationalMeshBuilder.h"

#include <boost/filesystem.hpp>

namespace FS = boost::filesystem;


// Static class data members
MeshBuilderMock                       ComputationalMeshBuilderTest::mesh_builder_;
ComputationalMeshBuilderTest::MeshPtr ComputationalMeshBuilderTest::mesh_;
BoundaryConditionCollection           ComputationalMeshBuilderTest::bc_;


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

namespace {
    bool flux_eval(ComputationalGridAccessor const & cgrid, ComputationalCell::Ptr const & cell, ComputationalFace::Ptr & face) {
        return true;
    }

}

void
ComputationalMeshBuilderTest::buildMeshTest() {
    ComputationalMeshBuilder cmesh(mesh_, bc_);

    // Temperature as cell-centered variable, will be solved for
    cmesh.addComputationalVariable("Temperature", flux_eval);

    // add user-defined node variable
    cmesh.addPassiveComputationalNodeVariable("node_var");

    cmesh.build();
}

void
ComputationalMeshBuilderTest::noActiveVarsTest() {
    ComputationalMeshBuilder cmesh(mesh_, bc_);
    ComputationalMesh::Ptr mesh(cmesh.build());
    bool is_null = mesh == std::nullptr_t();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Computational mesh build failure", true, is_null);
}

void
ComputationalMeshBuilderTest::addPassiveVarSameAsActiveVarTest() {
    ComputationalMeshBuilder cmesh(mesh_, bc_);

    cmesh.addComputationalVariable("Temperature", flux_eval);

    bool success = cmesh.addPassiveComputationalNodeVariable("Temperature");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cannot add a passive variable with same name as cell-centered variable", false, success);

    cmesh.addComputationalVariable("Temperature", flux_eval);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cannot add a passive variable with same name as cell-centered variable", false, success);

    success = cmesh.addPassiveComputationalCellVariable("Temperature");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cannot add a passive variable with same name as cell-centered variable", false, success);
}

void
ComputationalMeshBuilderTest::addPassiveVarTwiceTest() {
    ComputationalMeshBuilder cmesh(mesh_, bc_);

    cmesh.addComputationalVariable("Temperature", flux_eval);

    bool success = cmesh.addPassiveComputationalNodeVariable("Pressure");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Adding a user-defined variable failed", true, success);

    success = cmesh.addPassiveComputationalNodeVariable("Pressure");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cannot add the same passive variable twice", false, success);
}

void
ComputationalMeshBuilderTest::addPassiveVarForSeveralDifferentEntitiesTest() {
    ComputationalMeshBuilder cmesh(mesh_, bc_);

    cmesh.addComputationalVariable("Temperature", flux_eval);

    bool success = cmesh.addPassiveComputationalNodeVariable("Pressure");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Adding a user-defined variable failed", true, success);

    success = cmesh.addPassiveComputationalFaceVariable("Pressure");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Adding a user-defined variable failed", true, success);

    success = cmesh.addPassiveComputationalCellVariable("Pressure");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Adding a user-defined variable failed", true, success);
}

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
        bc_ = reader.getBoundaryConditions();
    }
}
