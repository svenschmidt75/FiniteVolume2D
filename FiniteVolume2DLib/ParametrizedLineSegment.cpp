#include "ParametrizedLineSegment.h"

#include "LineSegment.h"
#include "Util.h"

#include <boost/format.hpp>

#include <exception>
#include <cassert>


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
    assert(t >= 0 && t <= 1);
    if (!(t >= 0 && t <= 1)) {
        boost::format format = boost::format("ParametrizedLineSegment::get: Parameter t out of bounds, %1%!\n") % t;
        Util::error(format.str());

        // have to throw because we only return by reference
        throw std::out_of_range(format.str().c_str());
    }

    return p0_ + t * dir_;
}

double
ParametrizedLineSegment::length() const {
    return dir_.norm();
}
