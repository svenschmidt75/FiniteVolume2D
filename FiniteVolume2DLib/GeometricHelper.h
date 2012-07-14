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

class Vertex;
class LineSegment;


class DECL_SYMBOLS_2DLIB GeometricHelper {
public:
    enum IntersectionType : unsigned int {
        UNIQUE_INTERSECTION,
        OVERLAP,
        END
    };

public:
    static boost::optional<Vertex> intersect(LineSegment const & ls1, LineSegment const & ls2);
};
