#include "Cell.h"

#include <algorithm>


Cell::Cell(IGeometricEntity::Id_t cell_id, IGeometricEntity::Id_t mesh_id, EntityCollection<Face> const & faces)
    : cell_id_(cell_id), mesh_id_(mesh_id), faces_(faces) {


    // this is necessary since the compiler expects the references passed to the lambda expression
    // in the current scope. VS2010 compiler bug? See http://connect.microsoft.com/VisualStudio/feedback/details/560907/capturing-variables-in-nested-lambdas
    EntityCollection<Node> & v = nodes_;

    // extract nodes
    std::for_each(faces.begin(), faces.end(), [&v](Face::Ptr const & face) {
        // Extract and check all nodes before inserting into v
        EntityCollection<Node> const & nodes = face->getNodes();

        for (EntityCollection<Node>::size_type i = 0; i < nodes.size(); ++i) {
            Node::Ptr const & vert = nodes.getEntity(i);
            v.insertUnique(vert);
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

        delta += (v0->x() * v1->y() - v1->x() * v0->y());
    }
    return 0.5 * delta;
}

// Node
// Cell::centroid() const {
// //     GeometricHelper::cellCentroid(*this);
// 
// }

Cell::Ptr
Cell::create(IGeometricEntity::Id_t cell_id, IGeometricEntity::Id_t mesh_id, EntityCollection<Face> const & faces) {
    Cell::Ptr cell = Cell::Ptr(new Cell(cell_id, mesh_id, faces));
    return cell;
}
