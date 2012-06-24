/*
 * Name  : IComputationalMesh
 * Path  : 
 * Use   : facilitates unit testing
 * Author: Sven Schmidt
 * Date  : 06/23/2012
 */
#pragma once

#include "ComputationalCell.h"

#include "FiniteVolume2DLib/Thread.hpp"


class ComputationalVariableManager ;


class IComputationalMesh {
public:
    ~IComputationalMesh() {}

    virtual Thread<ComputationalCell> const &    getCellThread() const = 0;
    virtual ComputationalVariableManager const & getComputationalVariableManager() const = 0;
    virtual size_t                               getCellIndex(ComputationalCell::Ptr const & ccell) const = 0;
};
