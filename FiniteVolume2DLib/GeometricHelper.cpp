#include "GeometricHelper.h"

#include "Vertex.h"
#include "LineSegment.h"
#include "ParametrizedLineSegment.h"


namespace {
    GeometricHelper::IntersectionType
    intersectLineSegmentBase(LineSegment const & ls1, LineSegment const & ls2) {
        /* Intersect two line segments.
         * This code is based on p. 244,
         * Geometric Tools for Computer Graphics,
         * Philip J. Schneider; David H. Eberly
         */
        ParametrizedLineSegment pls1(ls1);





    }

}

boost::optional<Vertex>
intersect(LineSegment const & ls1, LineSegment const & ls2) {

}
