#include "ComputationalMeshSolverHelper.h"


ComputationalMeshSolverHelper::ComputationalMeshSolverHelper(IComputationalMesh const & cmesh)
    :
    cmesh_(cmesh) {}


bool
ComputationalMeshSolverHelper::solve() const {
    return true;
}

void
ComputationalMeshSolverHelper::setupMatrix() {
    /* For each ComputationalCell in the ComputationalMesh,
     * fill each COmputationalMolecule constitutes one
     * equation and hence one row in the linear system.
     */

//    Thread<ComputationalCell> const & cell_thread = cmesh_.

    // find number of cells and number of ComputationalVariables
    // to solve for per cell
    


}

IMatrix2D const &
ComputationalMeshSolverHelper::getMatrix() const {
    return *(m_.get());
}
