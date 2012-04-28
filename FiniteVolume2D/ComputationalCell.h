/*
 * Name  : ComputationalCell
 * Path  : ICell
 * Use   : Computational cell
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "FiniteVolume2DLib/ICell.h"
#include "FiniteVolume2DLib/Cell.h"
#include "FiniteVolume2DLib/Face.h"

#include "FiniteVolume2D/ComputationalMolecule.h"

#include <memory>


class ComputationalCell : public ICell {
public:
    typedef std::shared_ptr<ComputationalCell> Ptr;

public:
    explicit ComputationalCell(Cell::Ptr const & geometric_cell);

    // FROM IGeometricEntity
    IGeometricEntity::Id_t id() const;
    IGeometricEntity::Id_t meshId() const;

    // FROM ICell
    EntityCollection<Node> const & getNodes() const;
    EntityCollection<Face> const & getFaces() const;
    double                         volume() const;
    Vertex                         centroid() const;

    Vector                         faceNormal(Face::Ptr const & face) const;

    ComputationalMolecule &        getComputationalMolecule();

private:
    // the geometric partner cell
    Cell::Ptr geometric_cell_;

//    ComputationalMolecule computational_molecule_;
};
