/*
 * Name  : Ray
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 07/15/2012
 */
#pragma once

#include "DeclSpec.h"

#include "Vertex.h"
#include "ParametrizedLineSegment.h"


class DECL_SYMBOLS_2DLIB Ray {
public:
    explicit Ray(Vertex const & p0, Vertex const & p1);
    explicit Ray(LineSegment const & in);
    explicit Ray(ParametrizedLineSegment const & in);

    Vertex const & p0() const;
    Vertex const & p1() const;
    Vector         dir() const;
    Vertex         get(double t) const;

private:
    ParametrizedLineSegment ray_;
};
