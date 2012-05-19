#include "Math.h"

#include "Vector.h"
#include "Vertex.h"

#include <cmath>


double
Math::dot(Vector const & lhs, Vector const & rhs) {
    return lhs.x() * rhs.x() + lhs.y() * rhs.y();
}

double 
Math::dist(Vertex const & lhs, Vertex const & rhs) {
    double dx = lhs.x() - rhs.x();
    double dy = lhs.y() - rhs.y();
    return std::sqrt(dx * dx + dy * dy);
}
