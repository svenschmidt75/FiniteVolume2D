/*
 * Name  : ComputationalCell
 * Path  : ICell
 * Use   : Computational cell
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "FiniteVolume2DLib/ICell.h"
#include "ComputationalMoleculeManager.h"


class ComputationalCell : public ICell {
public:

private:
    /* A computational node may have a set of molecules,
     * but does not have to.
     */
    ComputationalMoleculeManager::Ptr molecule_;
};
