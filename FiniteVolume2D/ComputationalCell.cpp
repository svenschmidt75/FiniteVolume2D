#include "ComputationalCell.h"

#include "FiniteVolume2DLib/Util.h"

#include <exception>

#include <boost/format.hpp>


ComputationalCell::ComputationalCell(Cell::CPtr const & geometric_cell, EntityCollection<ComputationalFace const> const & faces)
    :
    geometric_cell_(geometric_cell), faces_(faces) {

    // extract nodes
    std::for_each(faces.begin(), faces.end(), [this](ComputationalFace::CPtr const & cface) {
        // Extract and check all nodes before inserting into v
        EntityCollection<ComputationalNode const> const & nodes = cface->getComputationalNodes();

        for (EntityCollection<ComputationalNode const>::size_type i = 0; i < nodes.size(); ++i) {
            ComputationalNode::CPtr const & vert = nodes.getEntity(i);
            nodes_.insertUnique(vert);
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

EntityCollection<Node const> const &
ComputationalCell::getNodes() const {
    return geometric_cell_->getNodes();
}

EntityCollection<Face const> const &
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
ComputationalCell::faceNormal(Face::CPtr const & face) const {
    return geometric_cell_->faceNormal(face);
}

Cell::CPtr const &
ComputationalCell::geometricEntity() const {
    return geometric_cell_;
}

EntityCollection<ComputationalNode const> const &
ComputationalCell::getComputationalNodes() const {
    return nodes_;
}

EntityCollection<ComputationalFace const> const &
ComputationalCell::getComputationalFaces() const {
    return faces_;
}

ComputationalVariable::CPtr const
ComputationalCell::getComputationalVariable(std::string const & name) const {
    /* cell-centered variables, will be solved for */
    auto it = cvars_.find(name);
    if (it == cvars_.end())
        return nullptr;
    return it->second;
}

void
ComputationalCell::addComputationalVariable(ComputationalVariable::CPtr const & cvar) {
    cvars_[cvar->getName()] = cvar;

    // also insert the corr. computational molecule
    addComputationalMolecule(cvar->getName());
}

ComputationalMolecule &
ComputationalCell::getComputationalMolecule(std::string const & name) {
    /* ComputationalMolecules are the same for active and passive variables, i.e.
     * there is no difference between those for cell-centered variables that will
     * be solved for and user-defined ones.
     */
    auto it = cm_.find(name);
    if (it == cm_.end()) {
        boost::format format = boost::format("ComputationalCell::getComputationalMolecule: No computational molecule found for \
            variable %1% and cell %2%!\n") % name % meshId();
        Util::error(format.str());

        // have to throw because we only return by reference
        throw std::exception(format.str().c_str());
    }
    return it->second;
}

void
ComputationalCell::addComputationalMolecule(std::string const & var_name) {
    cm_[var_name] = ComputationalMolecule(var_name);
}
