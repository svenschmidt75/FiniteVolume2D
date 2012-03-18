#include "MeshConnectivity.h"


Cell::Ptr
MeshConnectivity::getOtherCell(Face::Ptr & face, Cell::Ptr & cell) const {
    return face_connectivity_.getOtherCell(face, cell);
}

boost::optional<EntityCollection<Vertex>>
MeshConnectivity::getVertexNeighbors(Vertex::Ptr const & vertex) const {
    return vertex_connectivity_.getVertexNeighbors(vertex);
}

void
MeshConnectivity::insert(Face::Ptr const & face) {
    vertex_connectivity_.insert(face);
}

void
MeshConnectivity::insert(Cell::Ptr const & cell) {
    vertex_connectivity_.insert(cell);
    face_connectivity_.insert(cell);
}
