#include "ParametrizedLineSegment.h"

#include "LineSegment.h"


ParametrizedLineSegment::ParametrizedLineSegment(Vertex const & v0, Vertex const & v1)
    :
    v0_(v0),
    v1_(v1),
    dir_(v1 - v0) {}

ParametrizedLineSegment::ParametrizedLineSegment(LineSegment const & in)
    :
    v0_(in.v0()),
    v1_(in.v1()),
    dir_(in.v1() - in.v0()) {}

Vertex const &
ParametrizedLineSegment::v0() const {
    return v0_;
}

Vertex const &
ParametrizedLineSegment::v1() const {
    return v1_;
}

Vertex
ParametrizedLineSegment::get(double t) const {
    return v0_ + t * dir_;
}
