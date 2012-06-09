/*
 * Name  : IMeshConnectivity
 * Path  : 
 * Use   : Base class for describing mesh connectivity
 * Author: Sven Schmidt
 * Date  : 03/18/2012
 */
#pragma once

#include "Node.h"
#include "Face.h"
#include "Cell.h"
#include "EntityCollection.hpp"

#include <boost/optional.hpp>


class IMeshConnectivity {
public:
    virtual ~IMeshConnectivity() {}

    virtual void                                          insert(Face::CPtr const & face) = 0;
    virtual void                                          insert(Cell::CPtr const & cell) = 0;
    virtual boost::optional<EntityCollection<Node const>> getNodeNeighbors(Node::CPtr const & node) const = 0;
    virtual boost::optional<EntityCollection<Face const>> getFacesAttachedToNode(Node::CPtr const & node) const = 0;
    virtual boost::optional<EntityCollection<Cell const>> getCellsAttachedToNode(Node::CPtr const & node) const = 0;
    virtual boost::optional<EntityCollection<Cell const>> getCellsAttachedToFace(Face::CPtr const & face) const = 0;
    virtual Cell::CPtr                                    getOtherCell(Face::CPtr const & face, Cell::CPtr const & cell) const = 0;
};
