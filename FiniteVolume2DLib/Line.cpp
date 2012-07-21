#include "Line.h"

#include "LineSegment.h"
#include "ParametrizedLineSegment.h"
#include "Line.h"
#include "Ray.h"


Line::Line(Vertex const & p0, Vertex const & p1)
    :
    p0_(p0),
    p1_(p1),
    dir_(p1 - p0) {}

Line::Line(LineSegment const & in)
    :
    p0_(in.p0()),
    p1_(in.p1()),
    dir_(in.p1() - in.p0()) {}

Line::Line(ParametrizedLineSegment const & in)
    :
    p0_(in.p0()),
    p1_(in.p1()),
    dir_(in.p1() - in.p0()) {}

Line::Line(Ray const & in)
    :
    p0_(in.p0()),
    p1_(in.p1()),
    dir_(in.p1() - in.p0()) {}

Vertex const &
Line::p0() const {
    return p0_;
}

Vertex const &
Line::p1() const {
    return p1_;
}

Vector
Line::dir() const {
    return dir_;
}

Vertex
Line::get(double t) const {
    return p0_ + t * dir_;
}
