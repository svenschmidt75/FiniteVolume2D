#include "Vertex.h"

#include <numeric>


Vertex::Vertex() : x_(Vertex::undef()), y_(Vertex::undef()) {}

Vertex::Vertex(double x, double y) : x_(x), y_(y) {}

double
Vertex::x() const {
    return x_;
}

double
Vertex::y() const {
    return y_;
}

bool
Vertex::initialized() const {
    return x_ != std::numeric_limits<double>::signaling_NaN() && y_ != std::numeric_limits<double>::signaling_NaN();
}

double
Vertex::undef() {
    return std::numeric_limits<double>::quiet_NaN();
}

#include "internal/VertexOperators.cpp"
