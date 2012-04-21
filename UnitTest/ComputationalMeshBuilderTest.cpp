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
    bool flux_eval(IComputationalGridAccessor const & cgrid, ComputationalCell::Ptr const & cell, ComputationalFace::Ptr & face) {
        return true;
    }

}

void
ComputationalMeshBuilderTest::buildMeshTest() {
    ComputationalMeshBuilder cmesh(mesh_, bc_);

    // set methods for solving face flux
    cmesh.addComputationalVariable("T", "flux_T", flux_eval);

           

    //bool
//    phi_flux_evaluator(IComputationalGridAccessor const & cgrid, Cell::Ptr const & cell, Face::Ptr & face)


    // set phi comp. molecule on each comp. cell
    // set flux phi on each comp. face

//    ComputationalMesh::Ptr cmesh = cmesh.build();

//    PDESolver.solve(cmesh);
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
