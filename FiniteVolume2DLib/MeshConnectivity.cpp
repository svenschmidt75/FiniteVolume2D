#include "MeshConnectivity.h"


Cell::Ptr
MeshConnectivity::getOtherCell(Face::Ptr const & face, Cell::Ptr const & cell) const {
    return face_connectivity_.getOtherCell(face, cell);
}

boost::optional<EntityCollection<Vertex>>
MeshConnectivity::getVertexNeighbors(Vertex::Ptr const & vertex) const {
    return vertex_connectivity_.getVertexNeighbors(vertex);
}

boost::optional<EntityCollection<Face>>
MeshConnectivity::getFacesAttachedToVertex(Vertex::Ptr const & vertex) const {
    return vertex_connectivity_.getFacesAttachedToVertex(vertex);
}

boost::optional<EntityCollection<Cell>>
MeshConnectivity::getCellsAttachedToVertex(Vertex::Ptr const & vertex) const {
    return vertex_connectivity_.getCellsAttachedToVertex(vertex);
}

boost::optional<EntityCollection<Cell>>
MeshConnectivity::getCellsAttachedToFace(Face::Ptr const & face) const {
    return face_connectivity_.getCellsAttachedToFace(face);
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
