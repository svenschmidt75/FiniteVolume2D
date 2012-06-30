#include "ComputationalMeshSolverHelper.h"

#include "FiniteVolume2D/ComputationalCell.h"
#include "FiniteVolume2D/IComputationalMesh.h"
#include "FiniteVolume2D/ComputationalVariableManager.h"

#include "FiniteVolume2DLib/Thread.hpp"

#include "Solver/CSparseMatrixImpl.h"

#include <string>
#include <tuple>
#include <map>


ComputationalMeshSolverHelper::ComputationalMeshSolverHelper(IComputationalMesh const & cmesh)
    :
    cmesh_(cmesh) {}


void
ComputationalMeshSolverHelper::insertSolutionIntoCMesh(LinearSolver::RHS_t const & x) {
    /* The format of x is like this:
        * 
        * Row 1: Cell 1, Temperature
        * Row 2: Cell 1, Pressure
        * Row 3: Cell 3, Temperature
        * Row 4: Cell 3, Pressure
        * ...
        */

    // find number of ComputationalVariables to solve for per cell
    ComputationalVariableManager const & cvar_manager = cmesh_.getComputationalVariableManager();
    ComputationalVariableManager::size_type nvars = cvar_manager.size();


    auto it = x.begin();
    auto it_end = x.end();
    for (; it != it_end; ++it) {
        unsigned int pos = std::distance(x.begin(), it);

        // corresponding ComputationalCell index
        unsigned int cell_index = pos / nvars;

        // corresponding ComputationalVariable index
        short base_index = short(pos % nvars);

        unsigned int cvar_index = cvar_mapper_.findIndex(cell_index, base_index);

        cmesh_.setSolution(cell_index, cvar_index, *it);
    }
}

bool
ComputationalMeshSolverHelper::solve() {
    /* Setup the matrix by inserting the computational
     * molecules for each computational cell.
     */
    setupMatrix();

    // allocate right hand side
    LinearSolver::RHS_t x;
    x.resize(rhs_.size());

    // solve using SOR approach
    std::tie(std::ignore, x) = LinearSolver::sparseSOR(*m_, rhs_, 1.05);

    // insert the solution, x, into the ComputationalMolecule
    // of the corresponding ComputationalCell
    insertSolutionIntoCMesh(x);

    return true;
}

void
ComputationalMeshSolverHelper::fillRow(unsigned int row, ComputationalMolecule const & cm, CSparseMatrixImpl & A, ComputationalVariableManager const & cvar_manager) {
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
        unsigned int cell_index = cmesh_.getCellIndex(c);

        cvar_mapper_.insert(cell_index, base_index, unsigned int(cvar_index));


        unsigned int col = cell_index + base_index;
        A(row, col) = weight;
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

    rhs_.resize(ncols);


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

            fillRow(row, cm, A, cvar_manager);

            // fill in the r.h.s.
            rhs_[row] = cm.getSourceTerm().value();
        }
    }

    A.finalize();
}

IMatrix2D const &
ComputationalMeshSolverHelper::getMatrix() const {
    return *(m_.get());
}

LinearSolver::RHS_t const &
ComputationalMeshSolverHelper::getRHS() const {
    return rhs_;
}
