#include "Math.h"

#include "Vector.h"


double
Math::dot(Vector const & lhs, Vector const & rhs) {
    return lhs.x() * rhs.x() + lhs.y() * rhs.y();
}
