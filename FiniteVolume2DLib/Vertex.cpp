#include "Vertex.h"


Vertex::Vertex(IGeometricEntity::Id_t vertex_id, double x, double y) : vertex_id_(vertex_id), x_(x), y_(y) {}

IGeometricEntity::Id_t
Vertex::id() const {
    return vertex_id_;
}

double
Vertex::x() const {
    return x_;
}

double
Vertex::y() const {
    return y_;
}

Vertex::Ptr
Vertex::create(IGeometricEntity::Id_t vertex_id, double x, double y) {
    Vertex::Ptr vertex = Vertex::Ptr(new Vertex(vertex_id, x, y));
    return vertex;
}
