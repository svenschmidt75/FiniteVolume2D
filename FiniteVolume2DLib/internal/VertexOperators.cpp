/* Note: This file MUST be excluded from the project.
 *       It is included by Vertex.cpp ONLY!!!
 */
#include <cassert>


Vertex
operator+(Vertex const & lhs, Vertex const & rhs) {
    return Vertex(lhs.x() + rhs.x(), lhs.y() + rhs.y());
}

Vector
operator-(Vertex const & lhs, Vertex const & rhs) {
    return Vector(rhs.x() - lhs.x(), rhs.y() - lhs.y());
}

Vertex
operator/(Vertex const & in, double const value) {
    assert(value);
    return Vertex(in.x() / value, in.y() / value);
}
