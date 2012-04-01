#include "Vector.h"

#include <cmath>


Vector::Vector(double x, double y) : x_(x), y_(y) {}

double
Vector::x() const {
    return x_;
}

double
Vector::y() const {
    return y_;
}

double
Vector::norm() const {
    return std::sqrt(x_ * x_ + y_ * y_);
}
