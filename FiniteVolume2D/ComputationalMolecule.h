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


class DECL_SYMBOLS_2D ComputationalMolecule : public ComputationalMoleculeImpl {
public:
    /* The default constructor is necessary because
     * std::unordered_map::operator[] creates an entry
     * if one cannot be found.
     */
    ComputationalMolecule();
    explicit ComputationalMolecule(std::string const & var_name);

    // add the contributions of flux molecules to this one
    bool addMolecule(FluxComputationalMolecule const & in);

    // insert a solution value
    void   setValue(double value);
    double getValue() const;

private:
    // solution value for this ComputationalVariable (var_name)
    double value_;
};
