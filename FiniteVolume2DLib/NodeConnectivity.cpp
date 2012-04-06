#include "NodeConnectivity.h"

#include "Face.h"
#include "EntityCollection.hpp"


void
NodeConnectivity::insert(Face::Ptr const & face) {
    typedef EntityCollection<Node>::size_type size_type;
    EntityCollection<Node> const & nodes = face->getNodes();

    for (size_type i = 0; i < nodes.size(); ++i) {
        int next = (i + 1) % nodes.size();

        Node::Ptr v0 = nodes.getEntity(i);
        Node::Ptr v1 = nodes.getEntity(next);

        // insert node-node connection
        node_neighbors_[v0->id()].insertUnique(v1);
        node_neighbors_[v1->id()].insertUnique(v0);

        // insert node-face connection
        node_faces_[v0->id()].insertUnique(face);
    }
}

void
NodeConnectivity::insert(Cell::Ptr const & cell) {
    // it is assumed all the cell faces have already been inserted
    typedef EntityCollection<Node>::size_type size_type;
    EntityCollection<Node> const & nodes = cell->getNodes();

    for (size_type i = 0; i < nodes.size(); ++i) {
        Node::Ptr const & v0 = nodes.getEntity(i);

        // insert node-cell connection
        node_cells_[v0->id()].insert(cell);
    }
}

boost::optional<EntityCollection<Node>>
NodeConnectivity::getNodeNeighbors(Node::Ptr const & vertex) const {
    NodeNeighbor_t::const_iterator it = node_neighbors_.find(vertex->id());
    if (it == node_neighbors_.end())
        return boost::optional<EntityCollection<Node>>();

    return boost::optional<EntityCollection<Node>>(it->second);
}

boost::optional<EntityCollection<Face>>
NodeConnectivity::getFacesAttachedToNode(Node::Ptr const & vertex) const {
    NodeFace_t::const_iterator it = node_faces_.find(vertex->id());
    if (it == node_faces_.end())
        return boost::optional<EntityCollection<Face>>();

    return boost::optional<EntityCollection<Face>>(it->second);
}

boost::optional<EntityCollection<Cell>>
NodeConnectivity::getCellsAttachedToNode(Node::Ptr const & vertex) const {
    NodeCell_t::const_iterator it = node_cells_.find(vertex->id());
    if (it == node_cells_.end())
        return boost::optional<EntityCollection<Cell>>();

    return boost::optional<EntityCollection<Cell>>(it->second);
}
