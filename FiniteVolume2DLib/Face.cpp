#include "Face.h"

#include "Vector.h"


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

double
Face::area() const {
    Vertex::Ptr const & v0 = vertices_.getEntity(0);
    Vertex::Ptr const & v1 = vertices_.getEntity(1);

    double dx = v0->x() - v1->x();
    double dy = v0->y() - v1->y();
    double d = dx * dx + dy * dy;
    return sqrt(d);
}

Vector
Face::normal() const {
    Vertex::Ptr const & v0 = vertices_.getEntity(0);
    Vertex::Ptr const & v1 = vertices_.getEntity(1);

    /* Convention:
     * Normal and direction vector of
     * vertices v0 -> v1 yield a r.h.c.s.
     */
    double dx = v1->x() - v0->x();
    double dy = v1->y() - v0->y();

    Vector n(dy, -dx);
    return n;
}

Vertex
Face::centroid() const {
    Vertex::Ptr const & v0 = vertices_.getEntity(0);
    Vertex::Ptr const & v1 = vertices_.getEntity(1);

    /* Convention:
     * Normal and direction vector of
     * vertices v0 -> v1 yield a r.h.c.s.
     */
    double dx = v1->x() - v0->x();
    double dy = v1->y() - v0->y();

}

Face::Ptr
Face::create(IGeometricEntity::Id_t face_id, IGeometricEntity::Id_t mesh_id, bool on_boundary, EntityCollection<Vertex> const & vertices) {
    Face::Ptr vertex = Face::Ptr(new Face(face_id, mesh_id, on_boundary, vertices));
    return vertex;
}
