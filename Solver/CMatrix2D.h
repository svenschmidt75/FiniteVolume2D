/*
 * Name  : CMatrix2D
 * Path  : IMatrix2D
 * Use   : Implements a dense 2D matrix representation
 * Author: Sven Schmidt
 * Date  : 12/3/2011
 */
#pragma once

#include "DeclSpec.h"

#include "IMatrix2D.h"

#include <vector>


class CMatrix2D : public IMatrix2D {
public:
    // To be used as base class
    CMatrix2D(int rows, int cols);
    CMatrix2D(CMatrix2D const & in);
    CMatrix2D& operator=(CMatrix2D const & in);

    // FROM IMatrix2D
    int            getRows() const;
    int            getCols() const;
    double         operator()(unsigned int row, unsigned int col) const;
    double&        operator()(unsigned int row, unsigned int col);
    void           solve(Vec const & b, Vec & x) const;

    // Local Methods
    void           print() const;

    // Static Methods
    static CMatrix2D identity(int n);

private:
    std::vector<double> data_;

    // Rows and cols
    unsigned int rows_;
    unsigned int cols_;
};
