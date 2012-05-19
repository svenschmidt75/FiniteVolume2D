#include "Node.h"

#include "Vector.h"


Node::Node(IGeometricEntity::Id_t node_id, IGeometricEntity::Id_t mesh_id, IGeometricEntity::Entity_t entity_type, double x, double y)
    : node_id_(node_id), mesh_id_(mesh_id), entity_type_(entity_type), vertex_(Vertex(x, y)) {}

IGeometricEntity::Id_t
Node::id() const {
    return node_id_;
}

IGeometricEntity::Id_t
Node::meshId() const {
    return mesh_id_;
}

IGeometricEntity::Entity_t
Node::getEntityType() const {
    return entity_type_;
}

Vertex
Node::location() const {
    return vertex_;
}

Node::Ptr
Node::create(IGeometricEntity::Id_t node_id, IGeometricEntity::Id_t mesh_id, IGeometricEntity::Entity_t entity_type, double x, double y) {
    Node::Ptr node = Node::Ptr(new Node(node_id, mesh_id, entity_type, x, y));
    return node;
}
