/*
 * Name  : IConstIterator
 * Path  : 
 * Use   : Iterator to a const collection of objects
 * Author: Sven Schmidt
 * Date  : 01/29/2012
 */
#pragma once


template<typename T>
class IConstIterator {
public:
    ~IConstIterator() {}

    // Advance iterator to next object
    virtual void operator++() const = 0;

    // Return reference
    virtual T const & operator*() const = 0;
};
