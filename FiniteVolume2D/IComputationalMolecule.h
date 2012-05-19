/*
 * Name  : IComputationalMolecule
 * Path  : 
 * Use   : Base class for computational molecules (cell and face)
 * Author: Sven Schmidt
 * Date  : 04/21/2012
 */
#pragma once


class IComputationalMolecule {
public:
    virtual ~IComputationalMolecule() {}

    virtual void print() const = 0;
};
