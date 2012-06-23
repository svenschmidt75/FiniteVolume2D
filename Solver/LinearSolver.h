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
    static std::tuple<bool, CMatrix2D const, std::vector<double>> GaussElim(CMatrix2D const & A, std::vector<double> const & f);
    static std::tuple<bool, std::vector<double>>                  SOR(CMatrix2D const & A, std::vector<double> const & f, double omega);
    static std::tuple<bool, std::vector<double>>                  sparseSOR(CSparseMatrixImpl const & A, std::vector<double> const & f, double omega);
};
