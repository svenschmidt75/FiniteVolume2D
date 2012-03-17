#include "MeshConnectivity.h"


// Cell::Ptr
// MeshConnectivity::getOtherCell(Face::Ptr & face, Cell::Ptr & cell) const {
// 
// }
// 

// Use move semantics in EntityCollection<Vertex>???

boost::optional<EntityCollection<Vertex>>
MeshConnectivity::getVertexNeighbors(Vertex::Ptr & vertex) const {
    return vertex_connectivity_.getVertexNeighbors(vertex);
}

void
MeshConnectivity::insert(Face::Ptr const & face) {
    vertex_connectivity_.insert(face);
}


// EntityCollection<Face::Ptr> &
// MeshConnectivity::getFaces(Cell::Ptr & cell) {
// 
// }
