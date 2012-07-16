/*
 * Name  : VectorOperators
 * Path  :
 * Use   : Implements addition operators etc. on
 *         vectors.
 * Author: Sven Schmidt
 * Date  : 07/15/2012
 */
#pragma once

#include "../DeclSpec.h"

class Vector;
class Vertex;


Vector DECL_SYMBOLS_2DLIB operator-(Vertex const & lhs, Vertex const & rhs);

Vertex DECL_SYMBOLS_2DLIB operator+(Vertex const & p, Vector const & v);

Vector DECL_SYMBOLS_2DLIB operator*(double value, Vector const & v);
Vector DECL_SYMBOLS_2DLIB operator*(Vector const & v, double value);
Vector DECL_SYMBOLS_2DLIB operator/(Vector const & v, double value);
