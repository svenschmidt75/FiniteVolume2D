/*
 * Name  : SourceTerm
 * Path  : 
 * Use   : Contains the constant source term, i.e. that contribution
 *         that is not linked to any cell.
 * Author: Sven Schmidt
 * Date  : 05/19/2012
 */
#pragma once

#include "DeclSpec.h"


class DECL_SYMBOLS_2D SourceTerm {
public:
    SourceTerm();

    void operator+=(double const value);

    double value() const;

private:
    double value_;
};
