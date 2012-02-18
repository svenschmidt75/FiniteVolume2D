/*
 * Name  : ComputationalMoleculeConstIterator
 * Path  : 
 * Use   : Iterator for a const ComputationalMolecule
 * Author: Sven Schmidt
 * Date  : 01/29/2012
 */
#pragma once

#include "IConstIterator.hpp"
#include "ComputationalMolecule.h"

class ComputationalMoleculeConstIterator : public IConstIterator<ComputationalMolecule> {
public:
    class IteratorHelper : public IConstIteratorHelper<ComputationalMolecule> {
    public:
        IteratorHelper(ComputationalMolecule const &cm) : cm_(cm) {}

        // FROM IConstIteratorHelper
        ComputationalMolecule::Item const & get() const = 0;
        virtual T const & next() const = 0;

    };

public:
    ComputationalMoleculeConstIterator(IteratorHelper const & in);

    // FROM IConstIterator
    void                          operator++() const;
    ComputationalMolecule const & operator*() const;

private:
    // No assignment
    ComputationalMoleculeConstIterator & operator=(ComputationalMoleculeConstIterator const & in);

private:
    IteratorHelper helper_;
};
