/*
 * Name  : IComputationalMolecule
 * Path  : 
 * Use   : Base class for computational molecules (cell and face)
 * Author: Sven Schmidt
 * Date  : 04/21/2012
 */
#pragma once


class ComputationalNode;
class ComputationalFace;
class ComputationalCell;
class ComputationalVariableManager;


class IComputationalMolecule {
public:
    virtual ~IComputationalMolecule() {}

    virtual void print(ComputationalVariableManager const & cvar_mgr) const = 0;
    virtual void print(ComputationalNode const & cnode, ComputationalVariableManager const & cvar_mgr) const = 0;
    virtual void print(ComputationalFace const & cface, ComputationalVariableManager const & cvar_mgr) const = 0;
    virtual void print(ComputationalCell const & ccell, ComputationalVariableManager const & cvar_mgr) const = 0;
};
