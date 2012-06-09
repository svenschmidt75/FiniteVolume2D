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

ComputationalNode::Ptr const &
GeometricalEntityMapper::getComputationalNode(Node::Ptr const & node) const {
    /* As this is a const method, all member variables will be const as well.
     * Because std::map<>::operator[] is a non-const method, the compiler
     * will flag an error. Hence, node_map_ must be mutable.
     */
    return (node_map_[node->id()]).centity_;
}

ComputationalFace::Ptr const &
GeometricalEntityMapper::getComputationalFace(Face::Ptr const & face) const {
    return (face_map_[face->id()]).centity_;
}

ComputationalCell::Ptr const &
GeometricalEntityMapper::getComputationalCell(Cell::Ptr const & cell) const {
    return (cell_map_[cell->id()]).centity_;
}
