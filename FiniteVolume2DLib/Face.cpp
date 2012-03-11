#include "Face.h"


Face::Face(IGeometricEntity::Id_t face_id, IGeometricEntity::Id_t mesh_id, bool on_boundary, EntityCollection<Vertex> const & vertices)
    : face_id_(face_id), mesh_id_(mesh_id), on_boundary_(on_boundary), vertices_(vertices) {}

IGeometricEntity::Id_t
Face::id() const {
    return face_id_;
}

IGeometricEntity::Id_t
Face::meshId() const {
    return mesh_id_;
}

bool
Face::onBoundary() const {
    return on_boundary_;
}

EntityCollection<Vertex> const &
Face::getVertices() const {
    return vertices_;
}

Face::Ptr
Face::create(IGeometricEntity::Id_t face_id, IGeometricEntity::Id_t mesh_id, bool on_boundary, EntityCollection<Vertex> const & vertices) {
    Face::Ptr vertex = Face::Ptr(new Face(face_id, mesh_id, on_boundary, vertices));
    return vertex;
}
