#include "CMatrix2D.h"

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <iomanip>

#include <boost/assert.hpp>


CMatrix2D::CMatrix2D(int rows, int cols) : rows_(rows), cols_(cols) {
    // Initialize with 0
    data_.resize(rows_ * cols_, 0);
}

CMatrix2D::CMatrix2D(CMatrix2D const & in) : rows_(in.rows_), cols_(in.cols_), data_(in.data_) {}

CMatrix2D&
CMatrix2D::operator=(CMatrix2D const & in) {
    rows_ = in.rows_;
    cols_ = in.cols_;
    data_ = in.data_;
    return *this;
}

int
CMatrix2D::getRows() const {
    return rows_;
}

int
CMatrix2D::getCols() const {
    return cols_;
}

namespace {
    int row_major_offset(int row, int col, int const row_size) {
        return row * row_size + col;
    }

}

double const &
CMatrix2D::operator()(unsigned int row, unsigned int col) const {
    unsigned int index = row * cols_ + col;
    bool assert_cond = index >= 0 && index < rows_ * cols_;
    BOOST_ASSERT_MSG(assert_cond, "Index range error");
    if (!assert_cond)
        throw std::out_of_range("CMatrix2D::operator(): Out of range error");
    return data_[index];
}

double&
CMatrix2D::operator()(unsigned int row, unsigned int col) {
    unsigned int index = row * cols_ + col;
    bool assert_cond = index >= 0 && index < rows_ * cols_;
    BOOST_ASSERT_MSG(assert_cond, "Index range error");
    if (!assert_cond)
        throw std::out_of_range("CMatrix2D::operator(): Out of range error");
    return data_[index];
}

void
CMatrix2D::solve(Vec const & b, Vec & x) const {
    bool assert_cond = b.size() == cols_ && b.size() == x.size();
    BOOST_ASSERT_MSG(assert_cond, "Index range error");
    if (!assert_cond)
        throw std::out_of_range("CMatrix2D::solve(): Out of range error");

    for (unsigned int row = 0; row < rows_; ++row) {
        double tmp = 0;

        for (unsigned int col = 0; col < cols_; ++col)
            tmp += (*this)(row, col) * b[col];

        x[row] = tmp;
    }
}

void
CMatrix2D::print() const {
    for (unsigned int i = 0; i < rows_; ++i) {
        for (unsigned int j = 0; j < cols_; ++i) {
//            unsigned int index = i * cols_ + j;

            std::cout << std::setw(5) << (*this)(i, j);
        }
    }
}

CMatrix2D
CMatrix2D::identity(int n) {
    CMatrix2D ident(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            ident(i, j) = i == j ? 1.0 : 0.0;
    }
    return ident;
}
