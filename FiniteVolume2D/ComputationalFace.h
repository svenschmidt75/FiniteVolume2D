/*
 * Name  : ComputationalFace
 * Path  : IFace
 * Use   : Computational face
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "FiniteVolume2DLib/IFace.h"

#include "ComputationalMoleculeManager.h"
#include "BoundaryCondition.h"

#include <memory>


class ComputationalFace : public IFace {
public:
    typedef std::shared_ptr<ComputationalFace> Ptr;

private:
    /* A computational node may have a set of molecules,
     * but does not have to.
     */
    ComputationalMoleculeManager::Ptr molecule_;

    // boundary conditions, in case this is a boundary face
    BoundaryCondition::Ptr bc_;
};
