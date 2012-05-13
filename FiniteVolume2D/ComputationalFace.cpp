#include "ComputationalFace.h"

#include "FluxComputationalMolecule.h"

#include "FiniteVolume2DLib/Util.h"

#include <boost/format.hpp>

#include <exception>


ComputationalFace::ComputationalFace(Face::Ptr const & geometric_face, EntityCollection<ComputationalNode> const & cnodes) : geometric_face_(geometric_face), cnodes_(cnodes) , bc_(std::nullptr_t()) {}

IGeometricEntity::Id_t
ComputationalFace::id() const {
    return geometric_face_->id();
}

IGeometricEntity::Id_t
ComputationalFace::meshId() const {
    // computational faces have no mesh id
    return IGeometricEntity::undef();
}

IGeometricEntity::Entity_t
ComputationalFace::getEntityType() const {
    return geometric_face_->getEntityType();
}

EntityCollection<Node> const &
ComputationalFace::getNodes() const {
    return geometric_face_->getNodes();
}

double
ComputationalFace::area() const {
    return geometric_face_->area();
}

Vector
ComputationalFace::normal() const {
    return geometric_face_->normal();
}

Vertex
ComputationalFace::centroid() const {
    return geometric_face_->centroid();
}

EntityCollection<ComputationalNode> const &
ComputationalFace::getComputationalNodes() const {
    return cnodes_;
}

BoundaryCondition const &
ComputationalFace::getBoundaryCondition() const {
    return *bc_;
}

void
ComputationalFace::setBoundaryCondition(BoundaryCondition::Ptr const & bc) {
    bc_ = bc;
}

ComputationalMolecule &
ComputationalFace::getComputationalMolecule(std::string const & name) {
    auto it = cm_.find(name);
    if (it == cm_.end()) {
        boost::format format = boost::format("ComputationalFace::getComputationalMolecule: No computational molecule found for \
                                                variable %1% and face %2%!\n") % name % meshId();
        Util::error(format.str());

        // have to throw because we only return by reference
        throw std::exception(format.str().c_str());
    }
    return it->second;
}

void
ComputationalFace::addComputationalMolecule(std::string const & name) {
    cm_[name] = ComputationalMolecule(name);
}
