#include "GeometricalEntityMapper.h"

void
GeometricalEntityMapper::addNode(Node::Ptr const & node, ComputationalNode::Ptr const & cnode) {
    node_map_[node->id()] = Link<Node, ComputationalNode>(node, cnode);
}

void
GeometricalEntityMapper::addFace(Face::Ptr const & face, ComputationalFace::Ptr const & cface) {
    face_map_[face->id()] = Link<Face, ComputationalFace>(face, cface);
}

void
GeometricalEntityMapper::addCell(Cell::Ptr const & cell, ComputationalCell::Ptr const & ccell) {
    cell_map_[cell->id()] = Link<Cell, ComputationalCell>(cell, ccell);
}

ComputationalNode::Ptr &
GeometricalEntityMapper::getComputationalNode(Node::Ptr const & node) {
    return (node_map_[node->id()]).centity_;
}

ComputationalFace::Ptr &
GeometricalEntityMapper::getComputationalFace(Face::Ptr const & face) {
    return (face_map_[face->id()]).centity_;
}

ComputationalCell::Ptr &
GeometricalEntityMapper::getComputationalCell(Cell::Ptr const & cell) {
    return (cell_map_[cell->id()]).centity_;
}
