#include "ComputationalVariableTest.h"

#include "FiniteVolume2DLib/FaceManager.h"
#include "FiniteVolume2DLib/NodeManager.h"
#include "FiniteVolume2DLib/ASCIIMeshReader.h"
#include "FiniteVolume2DLib/Face.h"
#include "FiniteVolume2DLib/EntityCollection.hpp"
#include "FiniteVolume2DLib/Vector.h"


// Static class data members
MeshBuilderMock                    ComputationalVariableTest::mesh_builder_;
ComputationalVariableTest::MeshPtr ComputationalVariableTest::mesh_;


void
ComputationalVariableTest::setUp() {
    mesh_filename_ = "Data\\mesh_connectivity.mesh";

    initMesh();
}

void
ComputationalVariableTest::tearDown() {
}

void
ComputationalVariableTest::test() {
    Cell::Ptr const & c = mesh_builder_.cell_mgr_->getEntity(4);
    CPPUNIT_ASSERT_MESSAGE("Cell not found", c != NULL);

    ComputationalVariableManager mgr & cv_mgr = cmesh->getComputationalVariableManager();




    // create comp. variable
    ComputationalVariable::Ptr cvT = cv_mgr.create(c, "Temperature");










    ComputationalVariable::Ptr cvP = cv_mgr.create(c, "Pressure");

}

void
ComputationalVariableTest::initMesh() {
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
