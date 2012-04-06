/*
 * Name  : Vector
 * Path  : 
 * Use   : 2d vector
 * Author: Sven Schmidt
 * Date  : 04/01/2012
 */
#pragma once

#include "DeclSpec.h"

#include "Vertex.h"


class DECL_SYMBOLS Vector {
public:
    Vector(double x, double y);

    // unary minus
    Vector operator-() const;

    double x() const;
    double y() const;
    double norm() const;

private:
    Vertex vertex_;
};
