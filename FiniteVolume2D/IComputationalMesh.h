/*
 * Name  : IComputationalMesh
 * Path  : 
 * Use   : facilitates unit testing
 * Author: Sven Schmidt
 * Date  : 06/23/2012
 */
#pragma once

#include "ComputationalCell.h"
#include "ComputationalFace.h"

#include "FiniteVolume2DLib/IGeometricEntity.h"
#include "FiniteVolume2DLib/Thread.hpp"


class ComputationalVariableManager;


class IComputationalMesh {
public:
    ~IComputationalMesh() {}

    virtual Thread<ComputationalCell> const &    getCellThread() const = 0;
    virtual Thread<ComputationalFace> const &    getFaceThread(IGeometricEntity::Entity_t entity_type) const = 0;
    virtual ComputationalVariableManager const & getComputationalVariableManager() const = 0;
    virtual size_t                               getCellIndex(ComputationalCell::Ptr const & ccell) const = 0;
    virtual bool                                 setSolution(unsigned int cell_index, unsigned int cvar_index, double value) const = 0;
};
