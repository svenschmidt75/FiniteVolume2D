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
class Vertex;


class DECL_SYMBOLS_2DLIB Math {
public:
    static double dot(Vector const & lhs, Vector const & rhs);
    static double cross(Vector const & lhs, Vector const & rhs);
    static double dist(Vertex const & lhs, Vertex const & rhs);
};
