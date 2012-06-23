#include "ComputationalMeshSolverHelper.h"

#include "FiniteVolume2D/ComputationalCell.h"
#include "FiniteVolume2D/IComputationalMesh.h"
#include "FiniteVolume2D/ComputationalVariableManager.h"

#include "FiniteVolume2DLib/Thread.hpp"


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

    // find number of ComputationalCells
    Thread<ComputationalCell> const & cell_thread = cmesh_.getCellThread();
    Thread<ComputationalCell>::size_type ncells = cell_thread.size();

    // find number of ComputationalVariables to solve for per cell
    ComputationalVariableManager const & var_manager = cmesh_.getComputationalVariableManager();
    ComputationalVariableManager::size_type nvars = var_manager.size();




}

IMatrix2D const &
ComputationalMeshSolverHelper::getMatrix() const {
    return *(m_.get());
}
