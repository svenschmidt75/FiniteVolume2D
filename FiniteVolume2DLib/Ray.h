/*
 * Name  : Ray
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 07/15/2012
 */
#pragma once

#include "DeclSpec.h"

#include "Vertex.h"



class Ray {
public:
    explicit Ray(Vertex const & p0, Vertex const & p1);

    Vertex const & p0() const;
    Vertex const & p1() const;

private:
    Vertex p0_;
    Vertex p1_;
};
