/*
 * Name  : Line
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 07/21/2012
 */
#pragma once

#include "DeclSpec.h"

#include "Vector.h"
#include "Vertex.h"


class LineSegment;
class ParametrizedLineSegment;
class Ray;


class DECL_SYMBOLS_2DLIB Line {
public:
    explicit Line(Vertex const & p0, Vertex const & p1);
    explicit Line(LineSegment const & in);
    explicit Line(ParametrizedLineSegment const & in);
    explicit Line(Ray const & in);

    Vertex const & p0() const;
    Vertex const & p1() const;
    Vector         dir() const;
    Vertex         get(double t) const;

private:
    Vertex p0_;
    Vertex p1_;
    Vector dir_;
};
