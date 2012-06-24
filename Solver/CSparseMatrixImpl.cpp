#include "CSparseMatrixImpl.h"

#include <boost/assert.hpp>


CSparseMatrixImpl::CSparseMatrixImpl(int ncols) : ncols_(ncols), finalized_(false) {}

int
CSparseMatrixImpl::getRows() const {
    return 0;
}

int
CSparseMatrixImpl::getCols() const {
    return 0;
}

double const &
CSparseMatrixImpl::operator()(unsigned int i, unsigned int j) const {
    // i: x, j: y

    if (finalized_)
        throw std::exception("CSparseMatrixImpl::operator(): Matrix already finalized");

    Col_t & col = data_[i];
    return col[j];
}

double &
CSparseMatrixImpl::operator()(unsigned int i, unsigned int j) {
    // i: x, j: y

    if (finalized_)
        throw std::exception("CSparseMatrixImpl::operator(): Matrix already finalized");

    Col_t & col = data_[i];
    return col[j];
}

void
CSparseMatrixImpl::solve(Vec const & b, Vec & x) const {
    /* compute A x = b */
    if (!finalized_)
        throw std::exception("CSparseMatrixImpl::solve(): Matrix not yet finalized");

    bool assert_cond = b.size() == ncols_ && b.size() == x.size();
    BOOST_ASSERT_MSG(assert_cond, "Index range error");
    if (!assert_cond)
        throw std::out_of_range("CSparseMatrixImpl::solve(): Out of range error");

    int nrows = nelements_.size() - 1;

    // All rows
    for (int row = 0; row < nrows; ++row) {
        // Number of non-zero columns for this row
        int ncol = nelements_[row + 1] - nelements_[row];
        int offset = nelements_[row];

        double tmp = 0;

        // All non-zero columns
        for (int icol = 0; icol < ncol; ++icol) {
            int col = columns_[offset + icol];
            double a_ij = elements_[offset + icol];

            tmp += (a_ij * b[col]);
        }

        x[row] = tmp;
    }
}

void 
CSparseMatrixImpl::finalize() const {
    // Convert to compressed row storage format
    int nelements = 0;
    int nelements_total = 0;

    Row_t::const_iterator row_it(data_.begin());
    Row_t::const_iterator row_it_end(data_.end());

    // all rows
    for (; row_it != row_it_end; ++row_it) {
//        int row = (*row_it).first;

        Col_t const & col = (*row_it).second;

        // all columns
        Col_t::const_iterator col_it(col.begin());
        Col_t::const_iterator col_it_end(col.end());

        for (; col_it != col_it_end; ++col_it) {
            int col = (*col_it).first;
            double value = (*col_it).second;

            elements_.push_back(value);
            columns_.push_back(col);

            // Number of non-zero elements
            nelements++;
        }
        nelements_.push_back(nelements_total);
        nelements_total += nelements;
        nelements = 0;
    }
    nelements_.push_back(nelements_total);

    // Matrix has been finalized
    finalized_ = true;
}
