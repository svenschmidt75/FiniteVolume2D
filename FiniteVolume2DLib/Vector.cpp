#include "Vector.h"

#include <cmath>


Vector::Vector(double x, double y) : vertex_(Vertex(x, y)) {}

Vector
Vector::operator-() const {
    return Vector(-vertex_.x(), -vertex_.y());
}

double
Vector::x() const {
    return vertex_.x();
}

double
Vector::y() const {
    return vertex_.y();
}

double
Vector::norm() const {
    double x = vertex_.x();
    double y = vertex_.y();

    return std::sqrt(x * x + y * y);
}
