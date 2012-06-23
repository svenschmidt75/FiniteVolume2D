#include "ComputationalMeshSolverHelperTest.h"

#include "FiniteVolume2D/ComputationalMeshSolverHelper.h"

#include "internal/ComputationalMeshMock.h"


void
ComputationalMeshSolverHelperTest::setUp() {}

void
ComputationalMeshSolverHelperTest::tearDown() {}

void
ComputationalMeshSolverHelperTest::matrixConstructionTest() {
    ComputationalMeshMock cmesh;

    ComputationalMeshSolverHelper helper(cmesh);
    helper.setupMatrix();
    IMatrix2D const & m = helper.getMatrix();

}
