/*
 * Name  : ComputationalVariableManager
 * Path  : 
 * Use   : Creates computational variables.
 * Author: Sven Schmidt
 * Date  : 04/28/2012
 */
#pragma once

#include "DeclSpec.h"

#include "ComputationalCell.h"
#include "ComputationalVariableHolder.h"
#include "internal/ComputationalVariableManagerTypes.h"

/* We include this file here although a forward-declaration would suffice.
 * The reason is that the iterator class is considered internal and should
 * not be used outside this class. By including it here, any clients using
 * this class can use the iterator without the need to include the iterator
 * declaration.
 */
#include "internal/ComputationalVariableManagerIterator.h"

#include <memory>
#include <map>
#include <string>
#include <functional>
#include <tuple>


class ComputationalVariable;
class IComputationalGridAccessor;

#pragma warning(disable:4251)


class DECL_SYMBOLS_2D ComputationalVariableManager {

    friend class ComputationalVariableManagerIterator;

public:
    typedef std::shared_ptr<ComputationalVariableManager> Ptr;

    /* The following types are defined in ComputationalVariableManagerTypes.h as they also need
     * to be known to ComputationalVariableManagerIterator. Introduce them as "native" to this
     * class via typedefs.
     */

    /* type of flux evaluator */
    typedef ComputationalVariableManagerTypes::FluxEvaluator_t FluxEvaluator_t;

    typedef ComputationalVariableManagerIterator Iterator_t;

private:
    // type of internal data
    typedef ComputationalVariableManagerTypes::ComputationalVariableItem ComputationalVariableItem;
    typedef ComputationalVariableManagerTypes::Variables_t Variables_t;

public:
    typedef Variables_t::size_type size_type;

public:
    ComputationalVariableManager();

    // insert a computational variable into a cell
    std::shared_ptr<ComputationalVariable>         create(std::shared_ptr<ComputationalCell> const & cell, std::string const & name);

    bool                                           registerVariable(std::string const & name, FluxEvaluator_t const & flux_eval);

    std::shared_ptr<ComputationalVariable> const & getComputationalVariable(ComputationalVariable::Id_t id) const;

    short                                          getBaseIndex(std::string const & cvar_name) const;

    size_type                                      size() const;

    Iterator_t                                     begin() const;
    Iterator_t                                     end() const;

private:
    std::shared_ptr<ComputationalVariableHolder> cvar_holder_;

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
