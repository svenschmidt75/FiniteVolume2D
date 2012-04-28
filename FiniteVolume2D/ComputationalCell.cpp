#include "ComputationalCell.h"


ComputationalCell::ComputationalCell(Cell::Ptr const & geometric_cell, EntityCollection<ComputationalFace> const & faces)
    : geometric_cell_(geometric_cell), faces_(faces) {

    // this is necessary since the compiler expects the references passed to the lambda expression
    // in the current scope. VS2010 compiler bug? See http://connect.microsoft.com/VisualStudio/feedback/details/560907/capturing-variables-in-nested-lambdas
    EntityCollection<ComputationalNode> & v = nodes_;

    // extract nodes
    std::for_each(faces.begin(), faces.end(), [&v](ComputationalFace::Ptr const & cface) {
        // Extract and check all nodes before inserting into v
        EntityCollection<ComputationalNode> const & nodes = cface->getComputationalNodes();

        for (EntityCollection<ComputationalNode>::size_type i = 0; i < nodes.size(); ++i) {
            ComputationalNode::Ptr const & vert = nodes.getEntity(i);
            v.insertUnique(vert);
        }
    });
}

// FROM IGeometricEntity
IGeometricEntity::Id_t
ComputationalCell::id() const {
    return geometric_cell_->id();
}

IGeometricEntity::Id_t
ComputationalCell::meshId() const {
    // computational cells have no mesh id
    return IGeometricEntity::undef();
}

EntityCollection<Node> const &
ComputationalCell::getNodes() const {
    return geometric_cell_->getNodes();
}

EntityCollection<Face> const &
ComputationalCell::getFaces() const {
    return geometric_cell_->getFaces();
}

double
ComputationalCell::volume() const {
    return geometric_cell_->volume();
}

Vertex
ComputationalCell::centroid() const {
    return geometric_cell_->centroid();
}

Vector
ComputationalCell::faceNormal(Face::Ptr const & face) const {
    return geometric_cell_->faceNormal(face);
}

EntityCollection<ComputationalNode> const &
ComputationalCell::getComputationalNodes() const {
    return nodes_;
}

EntityCollection<ComputationalFace> const &
ComputationalCell::getComputationalFaces() const {
    return faces_;
}
