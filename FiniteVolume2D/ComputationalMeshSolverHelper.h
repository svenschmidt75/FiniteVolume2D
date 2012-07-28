/*
 * Name  : ComputationalMeshSolverHelper
 * Path  : 
 * Use   : Assembles the matrix and calls the solver.
 *         Fills in the solution into the computational mesh.
 * Author: Sven Schmidt
 * Date  : 06/23/2012
 */
#pragma once

#include "DeclSpec.h"

#include "ComputationalVariable.h"

#include "Solver/CSparseMatrixImpl.h"
#include "Solver/LinearSolver.h"

#include <memory>
#include <vector>

#include <boost/cstdint.hpp>


class IComputationalMesh;
class IMatrix2D;
class ComputationalMolecule;
class ComputationalVariableManager;


#pragma warning(disable:4251)


class DECL_SYMBOLS_2D ComputationalMeshSolverHelper {

    friend class ComputationalMeshSolverHelperTest;

public:
    ComputationalMeshSolverHelper(IComputationalMesh const & cmesh);

    bool solve();

private:

    class ComputationalVariableMapper {
    public:
        typedef boost::uint64_t CellIndex;
        typedef boost::uint64_t CVarIndex;
        typedef short           BaseIndex;

    public:
        void insert(CellIndex cell_index, BaseIndex base_index, CVarIndex cvar_index) {
            data_[std::make_tuple(cell_index, base_index)] = cvar_index;
        }

        CVarIndex findIndex(CellIndex cell_index, BaseIndex base_index) const {
            auto key = std::make_tuple(cell_index, base_index);
            auto it = data_.find(key);
            if (it == data_.end())
                return std::numeric_limits<CVarIndex>::max();
            return it->second;
        }

    private:
        std::map<std::tuple<CellIndex, BaseIndex>, CVarIndex> data_;
    };


private:
    ComputationalMeshSolverHelper(ComputationalMeshSolverHelper const & in);
    ComputationalMeshSolverHelper & operator=(ComputationalMeshSolverHelper const & in);

    // setup matrix
    void              setupMatrix();

    void              fillRow(boost::uint64_t row, ComputationalMolecule const & cm, CSparseMatrixImpl & A, ComputationalVariableManager const & cvar_manager);

    void              insertSolutionIntoCMesh(LinearSolver::RHS_t const & x);

    // for unit testing
    IMatrix2D const &           getMatrix() const;
    LinearSolver::RHS_t const & getRHS() const;

private:
    IComputationalMesh const &         cmesh_;

    std::unique_ptr<CSparseMatrixImpl> m_;

    LinearSolver::RHS_t                rhs_;

    ComputationalVariableMapper        cvar_mapper_;
};

#pragma warning(default:4251)
