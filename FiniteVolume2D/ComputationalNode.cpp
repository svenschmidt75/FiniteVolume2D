#include "ComputationalNode.h"

ComputationalNode::ComputationalNode(Node::Ptr const & geometric_node) : geometric_node_(geometric_node) {}

IGeometricEntity::Id_t
ComputationalNode::id() const {
    return geometric_node_->id();
}

IGeometricEntity::Id_t
ComputationalNode::meshId() const {
    // computational nodes have no mesh id
    return IGeometricEntity::undef();
}

IGeometricEntity::Entity_t
ComputationalNode::getEntityType() const {
    return geometric_node_->getEntityType();
}

double
ComputationalNode::x() const {
    return geometric_node_->x();
}

double
ComputationalNode::y() const {
    return geometric_node_->y();
}

Vector
ComputationalNode::vector() const {
    return geometric_node_->vector();
}

void
ComputationalNode::addComputationalMolecule(std::string const & var_name) {
    cm_[var_name] = ComputationalMolecule(var_name);
}
