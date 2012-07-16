#include "Ray.h"


Ray::Ray(Vertex const & p0, Vertex const & p1)
    :
    ray_(p0, p1) {}

Ray::Ray(LineSegment const & in)
    :
    ray_(in) {}

Ray::Ray(ParametrizedLineSegment const & in)
    :
    ray_(in) {}

Vertex const &
Ray::p0() const {
    return ray_.p0();
}

Vertex const &
Ray::p1() const {
    return ray_.p1();
}

Vector
Ray::dir() const {
    return ray_.dir();
}

Vertex
Ray::get(double t) const {
    return ray_.get(t);
}
