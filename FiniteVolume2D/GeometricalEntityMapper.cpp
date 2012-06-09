#include "GeometricalEntityMapper.h"


void
GeometricalEntityMapper::addNode(Node::CPtr const & node, ComputationalNode::CPtr const & cnode) {
    node_map_[node->id()] = Link<Node, ComputationalNode>(node, cnode);
}

void
GeometricalEntityMapper::addFace(Face::CPtr const & face, ComputationalFace::CPtr const & cface) {
    face_map_[face->id()] = Link<Face, ComputationalFace>(face, cface);
}

void
GeometricalEntityMapper::addCell(Cell::CPtr const & cell, ComputationalCell::CPtr const & ccell) {
    cell_map_[cell->id()] = Link<Cell, ComputationalCell>(cell, ccell);
}

ComputationalNode::CPtr const &
GeometricalEntityMapper::getComputationalNode(Node::CPtr const & node) const {
    /* As this is a const method, all member variables will be const as well.
     * Because std::map<>::operator[] is a non-const method, the compiler
     * will flag an error. Hence, node_map_ must be mutable.
     */
    return (node_map_[node->id()]).centity_;
}

ComputationalFace::CPtr const &
GeometricalEntityMapper::getComputationalFace(Face::CPtr const & face) const {
    return (face_map_[face->id()]).centity_;
}

ComputationalCell::CPtr const &
GeometricalEntityMapper::getComputationalCell(Cell::CPtr const & cell) const {
    return (cell_map_[cell->id()]).centity_;
}
