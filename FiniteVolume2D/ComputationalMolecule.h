/*
 * Name  : ComputationalMolecule
 * Path  : ComputationalMoleculeImpl
 * Use   : Computational molecules for comp. cells
 * Author: Sven Schmidt
 * Date  : 04/21/2012
 */
#pragma once

#include "ComputationalMoleculeImpl.h"


class ComputationalMolecule : public ComputationalMoleculeImpl {
public:
    /* The default constructor is necessary because
     * std::unordered_map::operator[] creates an entry
     * if one cannot be found.
     */
    ComputationalMolecule();
    explicit ComputationalMolecule(std::string const & var_name);
};
