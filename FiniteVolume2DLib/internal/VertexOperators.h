/*
 * Name  : VertexOperators
 * Path  :
 * Use   : Implements addition operators etc. on
 *         vertices.
 *         
 *         Note: This file MUST ONLY be included in Vertex.h
 * Author: Sven Schmidt
 * Date  : 05/19/2012
 */
#pragma once

#include "DeclSpec.h"


Vertex DECL_SYMBOLS_2DLIB operator+(Vertex const & lhs, Vertex const & rhs);
Vertex DECL_SYMBOLS_2DLIB operator/(Vertex const & in, double const value);
