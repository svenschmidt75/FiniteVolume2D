#include "VertexOperators.h"

#include "../Vertex.h"

#include <cassert>


Vertex
operator+(Vertex const & lhs, Vertex const & rhs) {
    return Vertex(lhs.x() + rhs.x(), lhs.y() + rhs.y());
}

Vertex
operator/(Vertex const & in, double const value) {
    assert(value);
    return Vertex(in.x() / value, in.y() / value);
}
