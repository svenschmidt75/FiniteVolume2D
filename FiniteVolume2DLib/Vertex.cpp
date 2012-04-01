#include "Vertex.h"

#include "Vector.h"


Vertex::Vertex(IGeometricEntity::Id_t vertex_id, IGeometricEntity::Id_t mesh_id, bool on_boundary, double x, double y)
    : vertex_id_(vertex_id), mesh_id_(mesh_id), on_boundary_(on_boundary), x_(x), y_(y) {}

IGeometricEntity::Id_t
Vertex::id() const {
    return vertex_id_;
}

IGeometricEntity::Id_t
Vertex::meshId() const {
    return mesh_id_;
}

bool
Vertex::onBoundary() const {
    return on_boundary_;
}

double
Vertex::x() const {
    return x_;
}

double
Vertex::y() const {
    return y_;
}

Vector
Vertex::vector() const {
    return Vector(x_, y_);
}

Vertex::Ptr
Vertex::create(IGeometricEntity::Id_t vertex_id, IGeometricEntity::Id_t mesh_id, bool on_boundary, double x, double y) {
    Vertex::Ptr vertex = Vertex::Ptr(new Vertex(vertex_id, mesh_id, on_boundary, x, y));
    return vertex;
}
