/*
 * Name  : IMeshConnectivity
 * Path  : 
 * Use   : Base class for describing mesh connectivity
 * Author: Sven Schmidt
 * Date  : 03/18/2012
 */
#pragma once

#include "Vertex.h"
#include "Face.h"
#include "Cell.h"
#include "EntityCollection.hpp"

#include <boost/optional.hpp>


class IMeshConnectivity {
public:
    virtual void                                      insert(Face::Ptr const & face) = 0;
    virtual void                                      insert(Cell::Ptr const & cell) = 0;
    virtual boost::optional<EntityCollection<Vertex>> getVertexNeighbors(Vertex::Ptr const & vertex) const = 0;
    virtual boost::optional<EntityCollection<Face>>   getFacesAttachedToVertex(Vertex::Ptr const & vertex) const = 0;
    virtual boost::optional<EntityCollection<Cell>>   getCellsAttachedToVertex(Vertex::Ptr const & vertex) const = 0;
    virtual boost::optional<EntityCollection<Cell>>   getCellsAttachedToFace(Face::Ptr const & face) const = 0;
    virtual Cell::Ptr                                 getOtherCell(Face::Ptr const & face, Cell::Ptr const & cell) const = 0;
};
