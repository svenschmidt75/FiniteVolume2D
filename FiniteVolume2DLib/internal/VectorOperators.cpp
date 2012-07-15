#include "VectorOperators.h"

#include "../Vector.h"
#include "../Vertex.h"


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
