/*
 * Name  : GeometricHelper
 * Path  : 
 * Use   : Helper functions for intersections etc.
 * Author: Sven Schmidt
 * Date  : 07/14/2012
 */
#pragma once

#include "DeclSpec.h"

#include <boost/optional.hpp>

#include <tuple>


class Vertex;
class LineSegment;
class Ray;
class Line;


// disable "warning C4480: nonstandard extension used: specifying underlying type for enum 'GeometricHelper::IntersectionType'"
#pragma warning(disable:4480)


class DECL_SYMBOLS_2DLIB GeometricHelper {
public:
    enum IntersectionType : unsigned int {
        UNIQUE_INTERSECTION,        // two line segments share a unique intersection point
        OVERLAP,                    // two line segments share part of the other's segment
        EMPTY,                      // no intersection point
        EMPTY_PARALLEL,             // two line segments are parallel
        END
    };

public:
    static boost::optional<Vertex> intersect(LineSegment const & ls1, LineSegment const & ls2);
    static boost::optional<Vertex> intersect(LineSegment const & ls, Ray const & ray);
    static boost::optional<Vertex> intersect(LineSegment const & ls, Line const & line);
};

#pragma warning(default:4480)
