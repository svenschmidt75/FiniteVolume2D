/*
 * Name  : ComputationalVariableManagerIterator
 * Path  : 
 * Use   : Allows iterator over comp. variable internals in ComputationalVariableManager
 * Author: Sven Schmidt
 * Date  : 04/29/2012
 */
#pragma once

#include "ComputationalVariableManager.h"

#include <string>


class ComputationalVariableManagerIterator {
    typedef ComputationalVariableManager::FluxEvaluator_t FluxEvaluator_t;
public:
    struct Item_t {
        // comp. variable name
        std::string name_;

        // Flux evaluator
        FluxEvaluator_t flux_eval;
    };

public:
    explicit ComputationalVariableManagerIterator(ComputationalVariableManager::Variables_t const & data);

    Item_t const &                               operator*() const;
    ComputationalVariableManagerIterator const & operator++() const;

private:
    // because of reference hold to internal data
    ComputationalVariableManagerIterator();
    ComputationalVariableManagerIterator(ComputationalVariableManagerIterator const &);
    ComputationalVariableManagerIterator & operator=(ComputationalVariableManagerIterator const &);

private:
    enum Type_t {NON_END, END};

private:
    ComputationalVariableManager::Variables_t const & data_;
};
