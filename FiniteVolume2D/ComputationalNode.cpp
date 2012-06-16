#include "ComputationalNode.h"

#include "FiniteVolume2DLib/Util.h"
#include "ComputationalVariableHolder.h"


#include <exception>

#include <boost/format.hpp>


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

Node::Ptr const &
ComputationalNode::geometricEntity() const {
    return geometric_node_;
}

Vertex
ComputationalNode::location() const {
    return geometric_node_->location();
}

ComputationalMolecule &
ComputationalNode::getComputationalMolecule(std::string const & name) {
    auto it = cm_.find(name);
    if (it == cm_.end()) {
        boost::format format = boost::format("ComputationalNode::getComputationalMolecule: No computational molecule found for \
                                                variable %1% and node %2%!\n") % name % meshId();
        Util::error(format.str());

        // have to throw because we only return by reference
        throw std::exception(format.str().c_str());
    }
    return it->second;
}

void
ComputationalNode::addComputationalMolecule(std::string const & name, ComputationalVariableHolder::Ptr const & cvar_holder) {
    cm_[name] = ComputationalMolecule(name, cvar_holder);
}
