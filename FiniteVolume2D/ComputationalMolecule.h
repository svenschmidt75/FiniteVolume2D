/*
 * Name  : ComputationalMolecule
 * Path  : ComputationalMoleculeImpl
 * Use   : Computational molecules for comp. cells
 * Author: Sven Schmidt
 * Date  : 04/21/2012
 */
#pragma once

#include "ComputationalMoleculeImpl.h"
#include "ComputationalVariable.h"

#include <boost/optional.hpp>

#include <unordered_map>


class ComputationalMolecule : public ComputationalMoleculeImpl {
public:
    typedef std::unordered_map<ComputationalVariable::Id_t, double>::value_type ComputationalMolecule_t;

public:
    explicit ComputationalMolecule(std::string const & var_name);

    // insert a dependency on cell id with weight
    void                    add(ComputationalVariable const & cvar, double weight);

    // Return the weight for an element
    boost::optional<double> getWeight(ComputationalVariable const & cvar) const;

    int                     size() const;

private:
    // Pair: Computational variable id, weight
    ComputationalMolecule_t data_;
};
