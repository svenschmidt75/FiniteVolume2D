/*
 * Name  : ComputationalMolecule
 * Path  : ComputationalMoleculeImpl
 * Use   : Computational molecules for comp. cells
 * Author: Sven Schmidt
 * Date  : 04/21/2012
 */
#pragma once

#include "DeclSpec.h"

#include "ComputationalMoleculeImpl.h"


class FluxComputationalMolecule;
class ComputationalVariableHolder;


class DECL_SYMBOLS_2D ComputationalMolecule : public ComputationalMoleculeImpl {
public:
    /* The default constructor is necessary because
     * std::unordered_map::operator[] creates an entry
     * if one cannot be found.
     */
    ComputationalMolecule();
    explicit ComputationalMolecule(std::string const & var_name, std::shared_ptr<ComputationalVariableHolder> const & cvar_holder);

    // add the contributions of flux molecules to this one
    bool addMolecule(FluxComputationalMolecule const & in);
};
