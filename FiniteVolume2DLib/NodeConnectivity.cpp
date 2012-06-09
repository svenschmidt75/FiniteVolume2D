#include "NodeConnectivity.h"

#include "Face.h"
#include "EntityCollection.hpp"


void
NodeConnectivity::insert(Face::CPtr const & face) {
    typedef EntityCollection<Node const>::size_type size_type;
    EntityCollection<Node const> const & nodes = face->getNodes();

    for (size_type i = 0; i < nodes.size(); ++i) {
        int next = (i + 1) % nodes.size();

        Node::CPtr const & v0 = nodes.getEntity(i);
        Node::CPtr const & v1 = nodes.getEntity(next);

        // insert node-node connection
        node_neighbors_[v0->id()].insertUnique(v1);
        node_neighbors_[v1->id()].insertUnique(v0);

        // insert node-face connection
        node_faces_[v0->id()].insertUnique(face);
    }
}

void
NodeConnectivity::insert(Cell::CPtr const & cell) {
    // it is assumed all the cell faces have already been inserted
    typedef EntityCollection<Node const>::size_type size_type;
    EntityCollection<Node const> const & nodes = cell->getNodes();

    for (size_type i = 0; i < nodes.size(); ++i) {
        Node::CPtr const & v0 = nodes.getEntity(i);

        // insert node-cell connection
        node_cells_[v0->id()].insert(cell);
    }
}

boost::optional<EntityCollection<Node const>>
NodeConnectivity::getNodeNeighbors(Node::CPtr const & vertex) const {
    NodeNeighbor_t::const_iterator it = node_neighbors_.find(vertex->id());
    if (it == node_neighbors_.end())
        return boost::optional<EntityCollection<Node const>>();

    return boost::optional<EntityCollection<Node const>>(it->second);
}

boost::optional<EntityCollection<Face const>>
NodeConnectivity::getFacesAttachedToNode(Node::CPtr const & vertex) const {
    NodeFace_t::const_iterator it = node_faces_.find(vertex->id());
    if (it == node_faces_.end())
        return boost::optional<EntityCollection<Face const>>();

    return boost::optional<EntityCollection<Face const>>(it->second);
}

boost::optional<EntityCollection<Cell const>>
NodeConnectivity::getCellsAttachedToNode(Node::CPtr const & vertex) const {
    NodeCell_t::const_iterator it = node_cells_.find(vertex->id());
    if (it == node_cells_.end())
        return boost::optional<EntityCollection<Cell const>>();

    return boost::optional<EntityCollection<Cell const>>(it->second);
}
