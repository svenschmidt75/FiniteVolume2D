/*
 * Name  : CSparseMatrixImpl
 * Path  : IMatrix2D
 * Use   : Implements a sparse 2D matrix representation.
 *         The matrix elements are first inserted and once finished,
 *         the internal matrix representation is converted into the
 *         compressed storage format.
 * Author: Sven Schmidt
 * Date  : 12/26/2011
 */
#pragma once

#include "DeclSpec.h"

#include "IMatrix2D.h"

#include <map>
#include <vector>

#include <boost/cstdint.hpp>


#pragma warning(disable:4251)
#pragma warning(disable:4275)


class DECL_SYMBOLS CSparseMatrixImpl : public IMatrix2D {

    friend struct LinearSolver;

public:
    CSparseMatrixImpl(boost::uint64_t ncols);

    // FROM IMatrix2D
    boost::uint64_t getRows() const;
    boost::uint64_t getCols() const;
    double          operator()(boost::uint64_t row, boost::uint64_t col) const;
    double &        operator()(boost::uint64_t row, boost::uint64_t col);
    void            solve(Vec const & b, Vec & x) const;

    // Local methods
    void finalize() const;
    void print() const;

private:
    typedef std::map<boost::uint64_t, double> Col_t;
    typedef std::map<boost::uint64_t, Col_t> Row_t;

private:
    // Number of columns
    boost::uint64_t ncols_;

    // row-major format
    mutable Row_t   data_;

    // Check whether the matrix has already been converted to the
    // compresses row storage format
    mutable bool    finalized_;

    /* Compressed row storage format
     * 
     * elements_ contains the non-zero elements in sequential order.
     * columns_ contains the column for the element.
     * nelements_: Contains the increasing number of elements per row.
     * 
     * The number of rows can be determined from nelements_: 
     * nelements_.size() == #rows + 1
     */
    mutable std::vector<double>          elements_;
    mutable std::vector<boost::uint64_t> columns_;
    mutable std::vector<boost::uint64_t> nelements_;
};

#pragma warning(default:4275)
#pragma warning(default:4251)
