#include "VertexConnectivity.h"

#include "Face.h"
#include "EntityCollection.hpp"


void
VertexConnectivity::insert(Face::Ptr const & face) {
    typedef EntityCollection<Vertex>::size_type size_type;
    EntityCollection<Vertex> const & vertices = face->getVertices();

    for (size_type i = 0; i < vertices.size(); ++i) {
        int next = (i + 1) % vertices.size();

        Vertex::Ptr v0 = vertices.getEntity(i);
        Vertex::Ptr v1 = vertices.getEntity(next);

        // insert vertex-vertex connection
        vertex_neighbors_[v0->id()].insertUnique(v1);
        vertex_neighbors_[v1->id()].insertUnique(v0);

        // insert vertex-face connection
        vertex_faces_[v0->id()].insertUnique(face);
    }
}

void
VertexConnectivity::insert(Cell::Ptr const & cell) {
    // it is assumed all the cell faces have already been inserted
    typedef EntityCollection<Vertex>::size_type size_type;
    EntityCollection<Vertex> const & vertices = cell->getVertices();

    for (size_type i = 0; i < vertices.size(); ++i) {
        Vertex::Ptr const & v0 = vertices.getEntity(i);

        // insert vertex-cell connection
        vertex_cells_[v0->id()].insert(cell);
    }
}

boost::optional<EntityCollection<Vertex>>
VertexConnectivity::getVertexNeighbors(Vertex::Ptr const & vertex) const {
    VertexNeighbor_t::const_iterator it = vertex_neighbors_.find(vertex->id());
    if (it == vertex_neighbors_.end())
        return boost::optional<EntityCollection<Vertex>>();

    return boost::optional<EntityCollection<Vertex>>(it->second);
}
