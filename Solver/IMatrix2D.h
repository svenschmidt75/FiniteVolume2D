/*
 * Name  : IMatrix2D
 * Path  : 
 * Use   : Base class for matrix implementations (dense vs sparse)
 * Author: Sven Schmidt
 * Date  : 12/3/2011
 */
#pragma once

#include <vector>


class IMatrix2D {
public:
    typedef std::vector<double> Vec;

public:
    // To be used as base class
    ~IMatrix2D() {}

    // Public matrix interface
    virtual int            getRows() const                     = 0;
    virtual int            getCols() const                     = 0;
    virtual double const & operator()(unsigned int row, unsigned int col) const  = 0;
    virtual double &       operator()(unsigned int row, unsigned int col)        = 0;
    virtual void           solve(Vec const & b, Vec & x) const = 0;
};
