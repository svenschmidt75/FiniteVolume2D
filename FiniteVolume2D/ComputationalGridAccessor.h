/*
 * Name  : ComputationalGridAccessor
 * Path  : 
 * Use   : Allows the user-supplied methods for flux evaluation
 *         to access the computational grid, for example to
 *         query the mesh connectivity etc.
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "ComputationalCell.h"
#include "ComputationalFace.h"


class ComputationalGridAccessor {
public:
    ComputationalCell::Ptr getOtherCell(ComputationalFace::Ptr const & face, ComputationalCell::Ptr const & cell);
};
