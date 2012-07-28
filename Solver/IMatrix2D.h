/*
 * Name  : IMatrix2D
 * Path  : 
 * Use   : Base class for matrix implementations (dense vs sparse)
 * Author: Sven Schmidt
 * Date  : 12/3/2011
 */
#pragma once

#include <vector>

#include <boost/cstdint.hpp>


class IMatrix2D {
public:
    typedef std::vector<double> Vec;

public:
    // To be used as base class
    ~IMatrix2D() {}

    // Public matrix interface
    virtual boost::uint64_t getRows() const                     = 0;
    virtual boost::uint64_t getCols() const                     = 0;
    virtual double          operator()(boost::uint64_t row, boost::uint64_t col) const  = 0;
    virtual double &        operator()(boost::uint64_t row, boost::uint64_t col)        = 0;
    virtual void            solve(Vec const & b, Vec & x) const = 0;

    virtual void            print() const = 0;
};
