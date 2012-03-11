#include "MeshConnectivity.h"

// Cell::Ptr
// MeshConnectivity::getOtherCell(Face::Ptr & face, Cell::Ptr & cell) const {
// 
// }
// 

// Use move semantics in EntityCollection<Vertex>???

boost::optional<EntityCollection<Vertex>>
MeshConnectivity::getVertexNeighbors(Vertex::Ptr & vertex) {
    VertexNeighbor_t::iterator it = vertex_neighbors_.find(vertex->id());
    if (it == vertex_neighbors_.end())
        return boost::optional<EntityCollection<Vertex>>();

    return boost::optional<EntityCollection<Vertex>>(it->second);
}

void
MeshConnectivity::insert(Face::Ptr const & face) {
    typedef EntityCollection<Vertex>::size_type size_type;
    EntityCollection<Vertex> const & vertices = face->getVertices();

    int index = 0;

    for (size_type i = 0; i < vertices.size(); ++i) {
        int next = (i + 1) % vertices.size();

        Vertex::Ptr v0 = vertices.getEntity(i);
        Vertex::Ptr v1 = vertices.getEntity(next);

        vertex_neighbors_[v0->id()].insert(v1);
        vertex_neighbors_[v1->id()].insert(v0);
    }
}


// EntityCollection<Face::Ptr> &
// MeshConnectivity::getFaces(Cell::Ptr & cell) {
// 
// }
