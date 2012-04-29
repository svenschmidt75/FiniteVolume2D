/*
 * Name  : ComputationalVariableManager
 * Path  : 
 * Use   : Creates computational variables.
 * Author: Sven Schmidt
 * Date  : 04/28/2012
 */
#pragma once

#include "DeclSpec.h"

#include "ComputationalVariableManagerIterator.h"
#include "ComputationalCell.h"
#include "ComputationalFace.h"

#include <memory>
#include <map>
#include <string>
#include <functional>
#include <tuple>


class ComputationalVariable;
class ComputationalGridAccessor;


#pragma warning(disable:4251)


class DECL_SYMBOLS_2D ComputationalVariableManager {

    friend class ComputationalVariableManagerIterator;

public:
    /* type of flux evaluator */
    typedef std::function<bool (ComputationalGridAccessor const & cgrid, ComputationalCell::Ptr const & cell, ComputationalFace::Ptr & face)> FluxEvaluator_t;

private:
    struct ComputationalVariableItem {
        // base index
        short           index;

        // Flux evaluator
        FluxEvaluator_t flux_eval;
    };

    typedef std::map<std::string, ComputationalVariableItem> Variables_t;

public:
    typedef Variables_t::size_type size_type;

public:
    ComputationalVariableManager();

    // insert a computational variable into a cell
    std::shared_ptr<ComputationalVariable> create(std::shared_ptr<ComputationalCell> const & cell, std::string const & name);

    bool                                   registerVariable(std::string const & name, FluxEvaluator_t const & flux_eval);

    short                                  getBaseIndex(std::string const & cvar_name) const;

    size_type                              size() const;

private:
    /* Each computational variable has a unique index.
     * This index is used in the computational molecules
     * and also in the sparse linear system.
     * Assume variables T(emperature) and P(ressure) have
     * been registered. Then, the base index is
     * T: 0
     * P: 1
     * This is used to order the linear system such that first,
     * all Ts appear, then all Ps.
     */
    Variables_t variables_;

    // number of comp. variables
    short       index_;

    /* After the first call of create, we do not allow further
     * additions (register) of computational variables, as this
     * would mess up all the indexing.
     */
    bool        is_finialized_;
};

#pragma warning(default:4251)
