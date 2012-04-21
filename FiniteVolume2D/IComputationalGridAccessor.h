/*
 * Name  : IComputationalGridAccessor
 * Path  : Allows the user-supplied methods for flux evaluation
 *         to access the computational grid, for example to
 *         query the mesh connectivity etc.
 * Use   : Mesh class
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once


class IComputationalGridAccessor {
public:
    virtual ~IComputationalGridAccessor() {}
};
