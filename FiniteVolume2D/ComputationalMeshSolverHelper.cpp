#include "ComputationalMeshSolverHelper.h"

#include "FiniteVolume2D/ComputationalCell.h"
#include "FiniteVolume2D/IComputationalMesh.h"
#include "FiniteVolume2D/ComputationalVariableManager.h"

#include "FiniteVolume2DLib/Thread.hpp"

#include "Solver/CSparseMatrixImpl.h"

#include <string>


ComputationalMeshSolverHelper::ComputationalMeshSolverHelper(IComputationalMesh const & cmesh)
    :
    cmesh_(cmesh) {}


bool
ComputationalMeshSolverHelper::solve() {
    /* Setup the matrix by inserting the computational
     * molecules for each computational cell.
     */
    setupMatrix();

//    std::tie(std::ignore, x) = LinearSolver::sparseSOR(sm_sparse, b, 1.05);



    return true;
}

namespace {
    void fillRow(unsigned int row, ComputationalMolecule const & cm, CSparseMatrixImpl & A, ComputationalVariableManager const & cvar_manager, IComputationalMesh const & cmesh) {
        ComputationalMolecule::Iterator_t cm_it  = cm.begin();
        ComputationalMolecule::Iterator_t cm_end = cm.end();

        for (; cm_it != cm_end; ++cm_it) {
            // get ComputationalVariable and weight
            ComputationalVariable::Id_t cvar_index = cm_it->first;
            ComputationalVariable::Ptr const & cvar = cvar_manager.getComputationalVariable(cvar_index);
            double const weight = cm_it->second;


            std::string const & cvar_name = cvar->getName();
            short base_index = cvar_manager.getBaseIndex(cvar_name);


            ComputationalCell::Ptr const & c = cvar->getCell();
            unsigned int cell_index = cmesh.getCellIndex(c);


            unsigned int col = cell_index + base_index;
            A(row, col) = weight;
        }

    }

}

void
ComputationalMeshSolverHelper::setupMatrix() {
    /* For each ComputationalCell in the ComputationalMesh,
     * each ComputationalMolecule constitutes one
     * equation and hence one row in the linear system.
     */

    // find number of ComputationalCells
    Thread<ComputationalCell> const & cell_thread = cmesh_.getCellThread();
    Thread<ComputationalCell>::size_type ncells = cell_thread.size();

    // find number of ComputationalVariables to solve for per cell
    ComputationalVariableManager const & cvar_manager = cmesh_.getComputationalVariableManager();
    ComputationalVariableManager::size_type nvars = cvar_manager.size();

    unsigned int ncols = ncells * nvars;


    // initialize the sparse matrix
    m_.reset(new CSparseMatrixImpl(ncols));
    CSparseMatrixImpl & A = *m_;



    for (Thread<ComputationalCell>::size_type cell_index = 0; cell_index < cell_thread.size(); ++cell_index) {
        ComputationalCell::Ptr const & ccell = cell_thread.getEntityAt(cell_index);

        unsigned int base_row = cell_index;


        ComputationalVariableManager::Iterator_t it     = cvar_manager.begin();
        ComputationalVariableManager::Iterator_t it_end = cvar_manager.end();

        for (; it != it_end; ++it) {
            std::string const & cvar_name = it->name;
            short cvar_index = cvar_manager.getBaseIndex(cvar_name);

            // row index in linear matrix
            unsigned int row = base_row + cvar_index;


            // get ComputationalMolecule for cell constituting an (independent) equation
            ComputationalMolecule const & cm = ccell->getComputationalMolecule(cvar_name);

            fillRow(row, cm, A, cvar_manager, cmesh_);
        }
    }
}

IMatrix2D const &
ComputationalMeshSolverHelper::getMatrix() const {
    return *(m_.get());
}
