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

// FluxComputationalMolecule &
// ComputationalFace::getComputationalMolecule(std::string const & flux_var) {
//     auto it = computational_molecules_.find(flux_var);
//     if (it == computational_molecules_.end()) {
//         boost::format format = boost::format("ComputationalFace::getComputationalMolecule: No computational molecule found for flux variable %1%!\n") % flux_var;
//         Util::error(format.str());
// 
//         // as this method returns by reference, we have to throw in
//         // case of an error
//         throw std::exception(format.str().c_str());
//     }
//     return it->second;
// }

// void
// ComputationalFace::setComputationalMolecule(FluxComputationalMolecule const & cm) {
//     computational_molecules_[cm.name()] = cm;
// }

void
ComputationalFace::addComputationalMolecule(std::string const & var_name) {
    cm_[var_name] = ComputationalMolecule(var_name);
}
