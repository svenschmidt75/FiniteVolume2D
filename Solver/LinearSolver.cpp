#include "LinearSolver.h"
#include "CMatrix2D.h"
#include "CSparseMatrixImpl.h"

#include <boost/assert.hpp>

namespace {
    int findPivotIndex(CMatrix2D const & A, int index, std::vector<int> const & row_row_map) {
        boost::uint64_t max_row = A.getRows();

        int pivot_index = -1;
        double pivot_value = std::numeric_limits<double>::min();
        double val;

        for (int row = 0; row < max_row; ++row) {
            int mapped_row = row_row_map[row];
            if (mapped_row < index)
                continue;

            val = fabs(A(mapped_row, index));
            if (val > pivot_value) {
                pivot_index = row;
                pivot_value = val;
            }
        }

        return pivot_index;
    }

}

std::tuple<bool, CMatrix2D const, std::vector<double> >
LinearSolver::GaussElim(CMatrix2D const & A, std::vector<double> const & f) {
    // Solve the equation A x = f using Gauss' elimination method with
    // row pivoting.
    // Description of algorithm: First, search for the largest element
    // in all following row and use it as the pivot element. This is to
    // reduce round-off errors. The matrix ACopy will be transformed
    // into the identity matrix, while the identity matrix, AInv, will
    // be transformed into the inverse.
    // A x =  Id y is successively transformed into A^{-1} A x = A^{-1} Id f,
    // where ACopy = Id and A^{-1} = AInv.
    boost::uint64_t max_col = A.getCols();
    boost::uint64_t max_row = A.getRows();
    bool success = false;
    CMatrix2D ACopy(A);
    CMatrix2D AInv(CMatrix2D::identity(max_col));
    std::vector<double> rhs(f);

    bool assert_cond = max_col == max_row;
    BOOST_ASSERT_MSG(assert_cond, "LinearSolver::GaussElim: Matrix must be quadratic");
    if (!assert_cond)
        throw std::out_of_range("LinearSolver::GaussElim: Matrix must be quadratic");

    // Create row-row map (because of pivoting)
    std::vector<int> row_row_map(max_row);
    for (int i = 0; i < max_row; ++i)
        row_row_map[i] = i;

    for (int col = 0; col < max_col; ++col) {
        // Find pivot element row index
        int pivot_index = findPivotIndex(ACopy, col, row_row_map);

        // Arrange rows due to row pivoting
        if (col != row_row_map[pivot_index]) {
            row_row_map[pivot_index] = col;
            row_row_map[row_row_map[col]] = pivot_index;
        }

        BOOST_ASSERT_MSG(row_row_map[pivot_index] == col, "LinearSolver::GaussElim: Pivoting error");
        double pivot_element = ACopy(pivot_index, col);

        // Matrix is singular
        if (pivot_element == 0.0)
            return std::make_tuple(false, AInv, rhs);

        // Divide pivot row by pivot element to set element to 1
        for (int i = 0; i < max_col; ++i) {
            // ACopy(pivot_index, i) = 0 for i < col
            if (i >= col) {
                double& val1 = ACopy(pivot_index, i);
                val1 /= pivot_element;
            }

            double& val2 = AInv(pivot_index, i);
            val2 /= pivot_element;
        }

        // Do same transformation on the rhs
        rhs[pivot_index] /= pivot_element;

        // Add pivot row to all other rows to reduce column col
        // to the corresponding identity matrix column.
        for (int current_row = 0; current_row < max_row; ++current_row) {
            if (current_row == pivot_index)
                continue;

            double val = - ACopy(current_row, col) / ACopy(pivot_index, col);

            for (int j = 0; j < max_col; ++j) {
                ACopy(current_row, j) += val * ACopy(pivot_index, j);
                AInv(current_row, j) += val * AInv(pivot_index, j);
            }

            // Do same transformation on the rhs
            rhs[current_row] += val * rhs[pivot_index];
        }
    }

    // Rearrange the rows in AInv due to pivoting
    for (int i = 0; i < max_row; ++i) {
        int row = row_row_map[i];

        if (row == i)
            continue;

        // Swap rows
        for (int j = 0; j < max_col; ++j) {
            double tmp = AInv(i, j);
            AInv(i, j) = AInv(row, j);
            AInv(row, j) = tmp;

            tmp = ACopy(i, j);
            ACopy(i, j) = ACopy(row, j);
            ACopy(row, j) = tmp;
        }

        // Swap rows on the rhs
        double tmp = rhs[i];
        rhs[i] = rhs[row];
        rhs[row] = tmp;

        row_row_map[i] = row_row_map[row];
        row_row_map[row] = row;
    }

    return std::make_tuple(success, AInv, rhs);
}

std::tuple<bool, IMatrix2D::Vec>
LinearSolver::SOR(CMatrix2D const & A, IMatrix2D::Vec const & f, double omega) {
    /* Implements the Successive OverReleaxation method from
     * "Templates for the Solution of Linear Systems:
     * Building Blocks for Iterative Methods"
     * 
     * Note: For this scheme to converge, matrix A must satisfy
     * the diagonal dominance requirement. Apparently, this is
     * true for all iterative schemes?
     */
    bool check_diagonally_dominant = true;

    // Rows/cols
    boost::uint64_t dim = A.getCols();

    // check if the system is diagonally dominant
    if (check_diagonally_dominant) {
        for (int i = 0; i < dim; ++i) {
            double sum = 0;

            for (int j = 0; j < dim; ++j) {
                if (i == j)
                    continue;

                // add the total value of the off-diagonal elements in row i
                sum += fabs(A(i, j));
            }

            if (sum > A(i, i))
                throw std::exception("LinearSolver::SOR: Matrix not diagonally dominant");
        }
    }

    IMatrix2D::Vec x(f.size(), 0);

    // Maximum allowed error
    double max_l2_norm = 1E-16;
    double l2_norm;

    // Iteration count
    int k = 0;

    do {
        l2_norm = 0;

        // All rows
        for (int i = 0; i < dim; ++i) {
            double sigma = 0;

            // Compute for the already known values in row i
            for (int j = 0; j <= i - 1; ++j)
                // sigma = sigma + a(i,j) x(j)^{k}
                sigma += A(i, j) * x[j];

            // Compute the new values in row i
            for (int j = i + 1; j < dim; ++j)
                sigma += A(i, j) * x[j];

            double a_ii = A(i, i);
            if (!a_ii)
                throw std::exception("LinearSolver::SOR: Matrix singular. Maybe too few independent equations?");

            sigma = (f[i] - sigma) / a_ii;

            double correction = omega * (sigma - x[i]);
            l2_norm += (correction * correction);

            x[i] += correction;
        }

        // Check error term
        l2_norm /= (dim + 1);
        l2_norm = sqrt(l2_norm);
    
        k++;

    } while (l2_norm > max_l2_norm && k < 10000);

    if (k == 10000)
        return std::make_tuple(false, x);

    return std::make_tuple(true, x);
}

std::tuple<bool, IMatrix2D::Vec>
LinearSolver::sparseSOR(CSparseMatrixImpl const & A, IMatrix2D::Vec const & f, double omega) {
    /* Implements the Successive OverReleaxation method from
     * "Templates for the Solution of Linear Systems:
     * Building Blocks for Iterative Methods"
     * 
     * Note: For this scheme to converge, matrix A must satisfy
     * the diagonal dominance requirement. Apparently, this is
     * true for all iterative schemes?
     */
    bool check_diagonally_dominant = false;

    // Number of rows
    typedef decltype(A.nelements_.size()) size_type;
    size_type nrows = A.nelements_.size() - 1;

    // check if the system is diagonally dominant
    if (check_diagonally_dominant) {
        for (int row = 0; row < nrows; ++row) {
            double a_ii = 0;
            bool has_diagonal_element = false;

            // Number of non-zero columns for this row
            boost::uint64_t ncol = A.nelements_[row + 1] - A.nelements_[row];
            boost::uint64_t offset = A.nelements_[row];

            double sum = 0;

            for (int icol = 0; icol < ncol; ++icol) {
                if (row == A.columns_[offset + icol]) {
                    a_ii = A.elements_[offset + icol];
                    has_diagonal_element = true;
                    continue;
                }

                // add the total value of the off-diagonal elements in row i
                double a_ij = A.elements_[offset + icol];
                sum += a_ij;
            }

            if (!has_diagonal_element || sum > a_ii)
                throw std::exception("LinearSolver::sparseSOR: Matrix not diagonally dominant");
        }
    }

    IMatrix2D::Vec x(f.size(), 0);

    // Maximum allowed error
    double max_l2_norm = 1E-16;
    double l2_norm;

    // Iteration count
    int k = 0;

    do {
        l2_norm = 0;

        // All rows
        for (int row = 0; row < nrows; ++row) {
            double a_ii = 0;

            // Number of non-zero columns for this row
            boost::uint64_t ncol = A.nelements_[row + 1] - A.nelements_[row];
            boost::uint64_t offset = A.nelements_[row];

            double sigma = 0;

            // The elements col <= row-1 have already been computed
            for (int icol = 0; icol < ncol; ++icol) {
                boost::uint64_t col = A.columns_[offset + icol];
                double a_ij = A.elements_[offset + icol];

                if (row == col) {
                    a_ii = a_ij;
                    continue;
                }

                // sigma = sigma + a(i,j) x(j)^{k}
                sigma += a_ij * x[col];
            }

            if (!a_ii)
                throw std::exception("LinearSolver::sparseSOR: Matrix singular. Maybe too few independent equations?");

            sigma = (f[row] - sigma) / a_ii;

            double correction = omega * (sigma - x[row]);
            l2_norm += (correction * correction);

            x[row] += correction;
        }

        // Check error term
        l2_norm /= (nrows + 1);
        l2_norm = sqrt(l2_norm);
    
        k++;

    } while (l2_norm > max_l2_norm && k < 10000);

    if (k == 10000)
        return std::make_tuple(false, x);

    return std::make_tuple(true, x);
}
