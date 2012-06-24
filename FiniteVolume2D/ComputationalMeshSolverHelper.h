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

#include "Solver/CSparseMatrixImpl.h"
#include "Solver/LinearSolver.h"

#include <memory>
#include <vector>


class IComputationalMesh;
class IMatrix2D;


#pragma warning(disable:4251)


class DECL_SYMBOLS_2D ComputationalMeshSolverHelper {

    friend class ComputationalMeshSolverHelperTest;

public:
    ComputationalMeshSolverHelper(IComputationalMesh const & cmesh);

    bool solve();

private:
    ComputationalMeshSolverHelper(ComputationalMeshSolverHelper const & in);
    ComputationalMeshSolverHelper & operator=(ComputationalMeshSolverHelper const & in);

    // setup matrix
    void              setupMatrix();

    // for unit testing
    IMatrix2D const &           getMatrix() const;
    LinearSolver::RHS_t const & getRHS() const;

private:
    IComputationalMesh const &         cmesh_;

    std::unique_ptr<CSparseMatrixImpl> m_;

    LinearSolver::RHS_t                rhs_;
};

#pragma warning(default:4251)
