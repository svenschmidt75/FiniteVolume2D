#include "Face.h"

#include "Vector.h"
#include "Math.h"


Face::Face(IGeometricEntity::Id_t face_id, IGeometricEntity::Id_t mesh_id, IGeometricEntity::Entity_t entity_type, EntityCollection<Node> const & nodes)
    : face_id_(face_id), mesh_id_(mesh_id), entity_type_(entity_type), nodes_(nodes) {}

IGeometricEntity::Id_t
Face::id() const {
    return face_id_;
}

IGeometricEntity::Id_t
Face::meshId() const {
    return mesh_id_;
}

IGeometricEntity::Entity_t
Face::getEntityType() const {
    return entity_type_;
}

EntityCollection<Node> const &
Face::getNodes() const {
    return nodes_;
}

double
Face::area() const {
    Node::Ptr const & v0 = nodes_.getEntity(0);
    Node::Ptr const & v1 = nodes_.getEntity(1);

    return Math::dist(v0->location(), v1->location());
}

Vector
Face::normal() const {
    Node::Ptr const & v0 = nodes_.getEntity(0);
    Node::Ptr const & v1 = nodes_.getEntity(1);

    /* Convention:
     * Normal and direction vector of
     * vertices v0 -> v1 yield a r.h.c.s.
     */
    double dx = v1->location().x() - v0->location().x();
    double dy = v1->location().y() - v0->location().y();

    Vector n(dy, -dx);
    return n;
}

Vertex
Face::centroid() const {
    Node::Ptr const & v0 = nodes_.getEntity(0);
    Node::Ptr const & v1 = nodes_.getEntity(1);

    /* Convention:
     * Normal and direction vector of
     * nodes v0 -> v1 yield a r.h.c.s.
     */
    double dx = v1->location().x() - v0->location().x();
    double dy = v1->location().y() - v0->location().y();

    // the centroid of a line segment is
    // its mid-point
    double x = v0->location().x() + 0.5 * dx;
    double y = v0->location().y() + 0.5 * dy;
    return Vertex(x, y);
}

Face::Ptr
Face::create(IGeometricEntity::Id_t face_id, IGeometricEntity::Id_t mesh_id, IGeometricEntity::Entity_t entity_type, EntityCollection<Node> const & nodes) {
    Face::Ptr face = Face::Ptr(new Face(face_id, mesh_id, entity_type, nodes));
    return face;
}
