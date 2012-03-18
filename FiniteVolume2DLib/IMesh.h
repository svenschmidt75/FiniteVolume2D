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
#include "Face.h"
#include "Cell.h"


class DECL_SYMBOLS IMesh {
public:
    ~IMesh() {}

    virtual void addVertex(Vertex::Ptr const & vertex) = 0;
    virtual void addFace(Face::Ptr const & face) = 0;
    virtual void addCell(Cell::Ptr const & cell) = 0;
};
