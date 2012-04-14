#include "ComputationalCell.h"


ComputationalCell::ComputationalCell(Cell::Ptr const & geometric_cell) : geometric_cell_(geometric_cell) {}

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
