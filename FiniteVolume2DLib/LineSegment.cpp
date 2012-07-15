#include "LineSegment.h"


LineSegment::LineSegment(Vertex const & p0, Vertex const & p1)
    :
    p0_(p0),
    p1_(p1) {}

Vertex const &
LineSegment::p0() const {
    return p0_;
}

Vertex const &
LineSegment::p1() const {
    return p1_;
}
