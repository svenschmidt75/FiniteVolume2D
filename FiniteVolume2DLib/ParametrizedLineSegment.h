/*
 * Name  : ParametrizedLineSegment
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 07/14/2012
 */
#pragma once

#include "DeclSpec.h"

#include "Vector.h"
#include "Vertex.h"



class ParametrizedLineSegment {
public:
    explicit ParametrizedLineSegment(Vertex const & v0, Vertex const & v1);

    Vertex const & v0() const;
    Vertex const & v1() const;
    Vertex         get(double t) const;


private:
    Vertex v0_;
    Vertex v1_;
    Vector dir_;
};
