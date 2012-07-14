#include "LineSegment.h"


LineSegment::LineSegment(Vertex const & v0, Vertex const & v1)
    :
    v0_(v0),
    v1_(v1) {}

Vertex const &
LineSegment::v0() const {
    return v0_;
}

Vertex const &
LineSegment::v1() const {
    return v1_;
}
