/*
 * Name  : ComputationalMoleculeImpl
 * Path  : IComputationalMolecule
 * Use   : Implements stuff common to ComputationalMolecule and
 *         FluxComputationalMolecule classes in order to avoid
 *         code duplication.
 * Author: Sven Schmidt
 * Date  : 04/21/2012
 */
#pragma once

#include "IComputationalMolecule.h"
#include "ComputationalVariable.h"

#include <boost/optional.hpp>

#include <string>
#include <unordered_map>


class ComputationalMoleculeImpl : public IComputationalMolecule {
public:
    typedef std::unordered_map<ComputationalVariable::Id_t, double> ComputationalMolecule_t;
    typedef ComputationalMolecule_t::size_type                      size_type;

public:
    std::string const &     name() const;

    // insert a dependency on cell id with weight
    void                    add(ComputationalVariable const & cvar, double weight);

    // Return the weight for an element
    boost::optional<double> getWeight(ComputationalVariable const & cvar) const;

    size_type               size() const;

protected:
    // protected constructor: class should only be used to derive
    //                        from it
    explicit ComputationalMoleculeImpl(std::string const & name);

private:
    std::string             name_;

    // Pair: Computational variable id, weight
    ComputationalMolecule_t data_;
};
