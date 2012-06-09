#include "Cell.h"

#include "Vector.h"
#include "Math.h"

#include <algorithm>


Cell::Cell(IGeometricEntity::Id_t cell_id, IGeometricEntity::Id_t mesh_id, EntityCollection<Face> const & faces)
    : cell_id_(cell_id), mesh_id_(mesh_id), faces_(faces) {


    // extract nodes
    std::for_each(faces.begin(), faces.end(), [this](Face::Ptr const & face) {
        // Extract and check all nodes before inserting into v
        EntityCollection<Node> const & nodes = face->getNodes();

        for (EntityCollection<Node>::size_type i = 0; i < nodes.size(); ++i) {
            Node::Ptr const & vert = nodes.getEntity(i);
            nodes_.insertUnique(vert);
        }
    });
}

IGeometricEntity::Id_t
Cell::id() const {
    return cell_id_;
}

IGeometricEntity::Id_t
Cell::meshId() const {
    return mesh_id_;
}

EntityCollection<Node> const &
Cell::getNodes() const {
    return nodes_;
}

EntityCollection<Face> const &
Cell::getFaces() const {
    return faces_;
}

double
Cell::volume() const {
    /* Geometric Tools for Computer Graphics,
     * Schneider and Eberly
     * p. 818, eq. (13.4)
     */
    typedef EntityCollection<Node>::size_type size_type;

    size_type n = nodes_.size();

    double delta = 0.0;

    for (size_type i = 0; i < n; ++i) {
        size_type next = (i + 1) % n;

        Node::Ptr const & v0 = nodes_.getEntity(i);
        Node::Ptr const & v1 = nodes_.getEntity(next);

        delta += (v0->location().x() * v1->location().y() - v1->location().x() * v0->location().y());
    }
    return 0.5 * delta;
}

Vertex
Cell::centroid() const {
    // a cell is a triangle
    double x = (nodes_[0]->location().x() + nodes_[1]->location().x() + nodes_[2]->location().x()) / 3.0;
    double y = (nodes_[0]->location().y() + nodes_[1]->location().y() + nodes_[2]->location().y()) / 3.0;
    return Vertex(x, y);
}

Vector
Cell::faceNormal(Face::Ptr const & face) const {
    /* A face is not owned by a cell. If two cells share a
     * face, the face normal has the opposite direction for
     * each cell.
     */

    Vector face_normal = face->normal();
    Vertex face_centroid = face->centroid();
    Vertex cell_centroid = centroid();

    // vector from cell to face centroid
    Vector cell_face_centroid = Vector(-cell_centroid.x() + face_centroid.x(), -cell_centroid.y() + face_centroid.y());

    // ensure both vector have same direction
    if (Math::dot(cell_face_centroid, face_normal) < 0)
        face_normal = -face_normal;

    return face_normal;
}

Cell::Ptr
Cell::create(IGeometricEntity::Id_t cell_id, IGeometricEntity::Id_t mesh_id, EntityCollection<Face> const & faces) {
    Cell::Ptr cell = Cell::Ptr(new Cell(cell_id, mesh_id, faces));
    return cell;
}
