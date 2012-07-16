#include "VectorOperators.h"

#include "..\Vector.h"
#include "..\Vertex.h"
#include "..\Util.h"

#include <boost/format.hpp>

#include <exception>
#include <cassert>


Vector
operator-(Vertex const & lhs, Vertex const & rhs) {
    return Vector(rhs.x() - lhs.x(), rhs.y() - lhs.y());
}

Vertex
operator+(Vertex const & p, Vector const & v) {
    return Vertex(p.x() + v.x(), p.y() + v.y());
}

Vector
operator*(double value, Vector const & v) {
    return Vector(v.x() * value, v.y() * value);
}

Vector
operator*(Vector const & v, double value) {
    return value * v;
}

Vector
operator/(Vector const & v, double value) {
    assert(value);
    if (!value) {
        boost::format format = boost::format("VectorOperators:: operator/(Vector const & v, double value): Division by zero!\n");
        Util::error(format.str());

        // have to throw because we only return by reference
        throw std::exception(format.str().c_str());
    }

    return Vector(v.x() / value, v.y() / value);
}
