/*
 * Name  : IConstIteratorHelper
 * Path  : 
 * Use   : Any iterator takes a derived object of this type
 *         which is used to implement the usual iterator
 *         interface.
 * Author: Sven Schmidt
 * Date  : 01/29/2012
 */
#pragma once


template<typename T>
class IConstIteratorHelper {
public:
    ~IConstIteratorHelper() {}

    // Any iterator implements the public iterator interface
    // with the help of these methods.
    virtual T const & get() const = 0;
    virtual T const & next() const = 0;
};
