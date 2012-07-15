#include "ParametrizedLineSegment.h"

#include "LineSegment.h"


ParametrizedLineSegment::ParametrizedLineSegment(Vertex const & p0, Vertex const & p1)
    :
    p0_(p0),
    p1_(p1),
    dir_(p1 - p0) {}

ParametrizedLineSegment::ParametrizedLineSegment(LineSegment const & in)
    :
    p0_(in.p0()),
    p1_(in.p1()),
    dir_(in.p1() - in.p0()) {}

Vertex const &
ParametrizedLineSegment::p0() const {
    return p0_;
}

Vertex const &
ParametrizedLineSegment::p1() const {
    return p1_;
}

Vector
ParametrizedLineSegment::dir() const {
    return dir_;
}

Vertex
ParametrizedLineSegment::get(double t) const {
    return p0_ + t * dir_;
}

double
ParametrizedLineSegment::length() const {
    return dir_.norm();
}
