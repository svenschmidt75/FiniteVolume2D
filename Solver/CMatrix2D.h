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

#include <boost/cstdint.hpp>


class CMatrix2D : public IMatrix2D {
public:
    // To be used as base class
    CMatrix2D(boost::uint64_t rows, boost::uint64_t cols);
    CMatrix2D(CMatrix2D const & in);
    CMatrix2D& operator=(CMatrix2D const & in);

    // FROM IMatrix2D
    boost::uint64_t getRows() const;
    boost::uint64_t getCols() const;
    double          operator()(boost::uint64_t row, boost::uint64_t col) const;
    double&         operator()(boost::uint64_t row, boost::uint64_t col);
    void            solve(Vec const & b, Vec & x) const;

    // Local Methods
    void           print() const;

    // Static Methods
    static CMatrix2D identity(boost::uint64_t n);

private:
    std::vector<double> data_;

    // Rows and cols
    boost::uint64_t rows_;
    boost::uint64_t cols_;
};
