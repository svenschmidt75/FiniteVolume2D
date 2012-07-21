#include "Ray.h"

#include "Util.h"

#include <boost/format.hpp>

#include <exception>
#include <cassert>


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
    assert(t >= 0);
    if (!(t >= 0)) {
        boost::format format = boost::format("Ray::get: Parameter t must be positive, %1%!\n") % t;
        Util::error(format.str());

        // have to throw because we only return by reference
        throw std::out_of_range(format.str().c_str());
    }
    return ray_.get(t);
}
