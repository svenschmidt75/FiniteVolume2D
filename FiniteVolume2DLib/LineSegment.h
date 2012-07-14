/*
 * Name  : LineSegment
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 07/14/2012
 */
#pragma once

#include "DeclSpec.h"

#include "Vertex.h"



class LineSegment {
public:
    explicit LineSegment(Vertex const & a, Vertex const & b);


private:
    Vertex a_;
    Vertex b_;
};
