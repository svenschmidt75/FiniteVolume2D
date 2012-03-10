/*
 * Name  : IMesh
 * Path  : 
 * Use   : Base class for geometric mesh and computational
 *         grid.
 * Author: Sven Schmidt
 * Date  : 01/29/2012
 */
#pragma once

#include "DeclSpec.h"

#include "Vertex.h"


class DECL_SYMBOLS IMesh {
public:
    ~IMesh() {}

    virtual void addVertex(Vertex::Ptr const & vertex, bool on_boundary) = 0;
};
