#include "MeshConnectivity.h"


Cell::CPtr
MeshConnectivity::getOtherCell(Face::CPtr const & face, Cell::CPtr const & cell) const {
    return face_connectivity_.getOtherCell(face, cell);
}

boost::optional<EntityCollection<Node const>>
MeshConnectivity::getNodeNeighbors(Node::CPtr const & node) const {
    return node_connectivity_.getNodeNeighbors(node);
}

boost::optional<EntityCollection<Face const>>
MeshConnectivity::getFacesAttachedToNode(Node::CPtr const & node) const {
    return node_connectivity_.getFacesAttachedToNode(node);
}

boost::optional<EntityCollection<Cell const>>
MeshConnectivity::getCellsAttachedToNode(Node::CPtr const & node) const {
    return node_connectivity_.getCellsAttachedToNode(node);
}

boost::optional<EntityCollection<Cell const>>
MeshConnectivity::getCellsAttachedToFace(Face::CPtr const & face) const {
    return face_connectivity_.getCellsAttachedToFace(face);
}

void
MeshConnectivity::insert(Face::CPtr const & face) {
    node_connectivity_.insert(face);
}

void
MeshConnectivity::insert(Cell::CPtr const & cell) {
    node_connectivity_.insert(cell);
    face_connectivity_.insert(cell);
}
