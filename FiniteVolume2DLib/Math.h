/*
 * Name  : Math
 * Path  : 
 * Use   : Helper functions
 * Author: Sven Schmidt
 * Date  : 04/06/2012
 */
#pragma once

#include "DeclSpec.h"

class Vector;


class DECL_SYMBOLS Math {
public:
    static double dot(Vector const & lhs, Vector const & rhs);
};
