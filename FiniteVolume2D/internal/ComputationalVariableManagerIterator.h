/*
 * Name  : ComputationalVariableManagerIterator
 * Path  : 
 * Use   : Allows iterator over comp. variable internals in ComputationalVariableManager
 * Author: Sven Schmidt
 * Date  : 04/29/2012
 */
#pragma once

#include "../DeclSpec.h"

#include "ComputationalVariableManagerTypes.h"

#include <string>


#pragma warning(disable:4251)


class DECL_SYMBOLS_2D ComputationalVariableManagerIterator {
private:
    /* The following types are defined in ComputationalVariableManagerTypes.h as they also need
     * to be known to ComputationalVariableManager. Introduce them as "native" to this
     * class via typedefs.
     */

    /* type of flux evaluator */
    typedef ComputationalVariableManagerTypes::FluxEvaluator_t FluxEvaluator_t;

    // type of internal data
    typedef ComputationalVariableManagerTypes::ComputationalVariableItem ComputationalVariableItem;
    typedef ComputationalVariableManagerTypes::Variables_t Variables_t;

public:
    struct Item_t {
        // comp. variable name
        std::string     name;

        // Flux evaluator
        FluxEvaluator_t flux_eval;
    };

public:
    explicit ComputationalVariableManagerIterator(Variables_t const & data);
    ComputationalVariableManagerIterator();
    ComputationalVariableManagerIterator(ComputationalVariableManagerIterator const & in);

    Item_t const &                               operator*() const;
    Item_t const *                               operator->() const;
    ComputationalVariableManagerIterator const & operator++() const;
    bool                                         operator==(ComputationalVariableManagerIterator const & in) const;
    bool                                         operator!=(ComputationalVariableManagerIterator const & in) const;

private:
    // because of reference hold to internal data
    ComputationalVariableManagerIterator & operator=(ComputationalVariableManagerIterator const &);

private:
    enum Type_t {NON_END, END};

private:
    Variables_t const &                 data_;
    static Variables_t                  end_data_;
    mutable Variables_t::const_iterator it_;
    mutable enum Type_t                 type_;

    // this is returned by the iterator, so we can return
    // both by reference and pointer
    mutable Item_t                      item_;
};

#pragma warning(default:4251)
