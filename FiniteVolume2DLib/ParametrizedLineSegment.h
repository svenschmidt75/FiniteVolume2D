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

class LineSegment;


class ParametrizedLineSegment {
public:
    explicit ParametrizedLineSegment(Vertex const & p0, Vertex const & p1);
    explicit ParametrizedLineSegment(LineSegment const & in);

    Vertex const & p0() const;
    Vertex const & p1() const;
    Vector         dir() const;
    Vertex         get(double t) const;
    double         length() const;


private:
    Vertex p0_;
    Vertex p1_;
    Vector dir_;
};
