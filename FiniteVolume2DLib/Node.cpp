#include "Node.h"

#include "Vector.h"


Node::Node(IGeometricEntity::Id_t node_id, IGeometricEntity::Id_t mesh_id, bool on_boundary, double x, double y)
    : node_id_(node_id), mesh_id_(mesh_id), on_boundary_(on_boundary), x_(x), y_(y) {}

IGeometricEntity::Id_t
Node::id() const {
    return node_id_;
}

IGeometricEntity::Id_t
Node::meshId() const {
    return mesh_id_;
}

bool
Node::onBoundary() const {
    return on_boundary_;
}

double
Node::x() const {
    return x_;
}

double
Node::y() const {
    return y_;
}

Vector
Node::vector() const {
    return Vector(x_, y_);
}

Node::Ptr
Node::create(IGeometricEntity::Id_t node_id, IGeometricEntity::Id_t mesh_id, bool on_boundary, double x, double y) {
    Node::Ptr node = Node::Ptr(new Node(node_id, mesh_id, on_boundary, x, y));
    return node;
}
