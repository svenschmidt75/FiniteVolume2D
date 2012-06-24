/*
 * Name  : LinearSolver
 * Path  : 
 * Use   : A collection of matrix equation solvers
 * Author: Sven Schmidt
 * Date  : 12/3/2011
 */
#pragma once

#include "DeclSpec.h"

#include <tuple>
#include <vector>


class CMatrix2D;
class CSparseMatrixImpl;


struct DECL_SYMBOLS LinearSolver {
    typedef std::vector<double> RHS_t;

    static std::tuple<bool, CMatrix2D const, RHS_t> GaussElim(CMatrix2D const & A, RHS_t const & f);
    static std::tuple<bool, RHS_t>                  SOR(CMatrix2D const & A, RHS_t const & f, double omega);
    static std::tuple<bool, RHS_t>                  sparseSOR(CSparseMatrixImpl const & A, RHS_t const & f, double omega);
};
