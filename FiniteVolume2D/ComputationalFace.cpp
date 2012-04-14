#include "ComputationalFace.h"


ComputationalFace::ComputationalFace(Face::Ptr const & geometric_face) : geometric_face_(geometric_face) {}

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
