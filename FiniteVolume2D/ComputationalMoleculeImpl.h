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

#include "DeclSpec.h"

#include "IComputationalMolecule.h"
#include "ComputationalVariable.h"
#include "SourceTerm.h"

#include <boost/optional.hpp>

#include <string>
#include <unordered_map>
#include <memory>


class ComputationalVariableHolder;


#pragma warning(disable:4251)
#pragma warning(disable:4275)


class DECL_SYMBOLS_2D ComputationalMoleculeImpl : public IComputationalMolecule {
public:
    typedef std::unordered_map<ComputationalVariable::Id_t, double> ComputationalMolecule_t;
    typedef ComputationalMolecule_t::size_type                      size_type;

public:
    // FROM IComputationalMolecule
    void                    print() const;


    std::string const &     name() const;

    // insert a dependency on cell id with weight
    void                    add(ComputationalVariable const & cvar, double weight);

    // Return the weight for an element
    boost::optional<double> getWeight(ComputationalVariable const & cvar) const;

    size_type               size() const;
    bool                    empty() const;

    SourceTerm &            getSourceTerm();

protected:
    // protected constructor: class should only be used to derive
    //                        from it
    explicit ComputationalMoleculeImpl(std::string const & name, std::shared_ptr<ComputationalVariableHolder> const & cvar_holder);

    bool addMolecule(ComputationalMoleculeImpl & in) const;

private:
    SourceTerm const &      getSourceTerm() const;

private:
    std::string                                  name_;

    // Pair: Computational variable id, weight
    ComputationalMolecule_t                      data_;

    // Source term, i.e. the constant value
    SourceTerm                                   source_term_;

    std::shared_ptr<ComputationalVariableHolder> cvar_holder_;
};

#pragma warning(default:4275)
#pragma warning(default:4251)
