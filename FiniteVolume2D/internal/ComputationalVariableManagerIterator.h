/*
 * Name  : ComputationalVariableManagerIterator
 * Path  : 
 * Use   : Allows iterator over comp. variable internals in ComputationalVariableManager
 * Author: Sven Schmidt
 * Date  : 04/29/2012
 */
#pragma once

#include "ComputationalVariableManagerTypes.h"

#include <string>


class ComputationalVariableManagerIterator {
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
        std::string name_;

        // Flux evaluator
        FluxEvaluator_t flux_eval;
    };

public:
    explicit ComputationalVariableManagerIterator(Variables_t const & data);

    Item_t const &                               operator*() const;
    ComputationalVariableManagerIterator const & operator++() const;

private:
    // because of reference hold to internal data
//    ComputationalVariableManagerIterator();
 //   ComputationalVariableManagerIterator(ComputationalVariableManagerIterator const &);
    ComputationalVariableManagerIterator & operator=(ComputationalVariableManagerIterator const &);

private:
    enum Type_t {NON_END, END};

private:
    Variables_t const & data_;
};
