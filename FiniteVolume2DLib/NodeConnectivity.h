/*
 * Name  : NodeConnectivity
 * Path  : 
 * Use   : Node connectivity
 * Author: Sven Schmidt
 * Date  : 03/17/2012
 */
#pragma once

#include "Face.h"
#include "Cell.h"

#include <map>

#include <boost/optional.hpp>

class Node;


class NodeConnectivity {
public:
    void                                    insert(Face::Ptr const & face);
    void                                    insert(Cell::Ptr const & cell);
    boost::optional<EntityCollection<Node>> getNodeNeighbors(Node::Ptr const & vertex) const;
    boost::optional<EntityCollection<Face>> getFacesAttachedToNode(Node::Ptr const & vertex) const;
    boost::optional<EntityCollection<Cell>> getCellsAttachedToNode(Node::Ptr const & vertex) const;

private:
    // the neighboring nodes
    typedef std::map<IGeometricEntity::Id_t, EntityCollection<Node>> NodeNeighbor_t;
    NodeNeighbor_t node_neighbors_;

    // attached faces
    typedef std::map<IGeometricEntity::Id_t, EntityCollection<Face>> NodeFace_t;
    NodeFace_t node_faces_;

    // attached cells
    typedef std::map<IGeometricEntity::Id_t, EntityCollection<Cell>> NodeCell_t;
    NodeCell_t node_cells_;
};
