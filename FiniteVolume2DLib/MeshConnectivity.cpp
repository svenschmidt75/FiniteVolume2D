#include "MeshConnectivity.h"


Cell::Ptr
MeshConnectivity::getOtherCell(Face::Ptr const & face, Cell::Ptr const & cell) const {
    return face_connectivity_.getOtherCell(face, cell);
}

boost::optional<EntityCollection<Node>>
MeshConnectivity::getNodeNeighbors(Node::Ptr const & node) const {
    return node_connectivity_.getNodeNeighbors(node);
}

boost::optional<EntityCollection<Face>>
MeshConnectivity::getFacesAttachedToNode(Node::Ptr const & node) const {
    return node_connectivity_.getFacesAttachedToNode(node);
}

boost::optional<EntityCollection<Cell>>
MeshConnectivity::getCellsAttachedToNode(Node::Ptr const & node) const {
    return node_connectivity_.getCellsAttachedToNode(node);
}

boost::optional<EntityCollection<Cell>>
MeshConnectivity::getCellsAttachedToFace(Face::Ptr const & face) const {
    return face_connectivity_.getCellsAttachedToFace(face);
}

void
MeshConnectivity::insert(Face::Ptr const & face) {
    node_connectivity_.insert(face);
}

void
MeshConnectivity::insert(Cell::Ptr const & cell) {
    node_connectivity_.insert(cell);
    face_connectivity_.insert(cell);
}
