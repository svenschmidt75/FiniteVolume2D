/*
 * Name  : ComputationalVariableManagerTypes
 * Path  : 
 * Use   : Only to be used by ComputationalVariableManager and ComputationalVariableManagerIterator
 * Author: Sven Schmidt
 * Date  : 04/29/2012
 */
#pragma once

#include <functional>
#include <memory>
#include <map>


class ComputationalGridAccessor;
class ComputationalCell;
class ComputationalFace;


class ComputationalVariableManagerTypes {
public:
    /* type of flux evaluator */
    typedef std::function<bool (ComputationalGridAccessor const & cgrid, std::shared_ptr<ComputationalCell> const & cell, std::shared_ptr<ComputationalFace> const & face)> FluxEvaluator_t;

    struct ComputationalVariableItem {
        // base index
        short           index;

        // Flux evaluator
        FluxEvaluator_t flux_eval;
    };

    // type of data the iterator iterates over
    typedef std::map<std::string, ComputationalVariableItem> Variables_t;
};
