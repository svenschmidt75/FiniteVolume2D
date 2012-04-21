/*
 * Name  : Vertex
 * Path  : 
 * Use   : Represents a 2d vertex.
 * Author: Sven Schmidt
 * Date  : 04/06/2012
 */
#pragma once

#include "DeclSpec.h"


class DECL_SYMBOLS_2DLIB Vertex {
public:
    Vertex();
    Vertex(double x, double y);

    double x() const;
    double y() const;

    bool initialized() const;

private:
    static double undef();

private:
    double x_;
    double y_;
};
