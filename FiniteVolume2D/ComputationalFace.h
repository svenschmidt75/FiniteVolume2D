/*
 * Name  : ComputationalFace
 * Path  : IFace
 * Use   : Computational face
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "FiniteVolume2DLib/IFace.h"
#include "FiniteVolume2DLib/Face.h"

#include "ComputationalMoleculeManager.h"
#include "BoundaryCondition.h"

#include <memory>

class FluxComputationalMolecule;


class ComputationalFace : public IFace {
public:
    typedef std::shared_ptr<ComputationalFace> Ptr;

public:
    explicit ComputationalFace(Face::Ptr const & geometric_face);

    // FROM IGeometricEntity
    IGeometricEntity::Id_t id() const;
    IGeometricEntity::Id_t meshId() const;

    // FROM IFace
    IGeometricEntity::Entity_t     getEntityType() const;
    EntityCollection<Node> const & getNodes() const;
    double                         area() const;
    Vector                         normal() const;
    Vertex                         centroid() const;

    BoundaryCondition const &      getBoundaryCondition() const;
    void                           setBoundaryCondition(BoundaryCondition::Ptr const & bc);

    FluxComputationalMolecule &    getComputationalMolecule(std::string const & flux_var);
    void                           setComputationalMolecule(FluxComputationalMolecule const & cm);

private:
    typedef std::map<std::string, FluxComputationalMolecule> FluxComputationalMolecule_t;

private:
    // the geometric partner face
    Face::Ptr geometric_face_;

    // computational molecules of face
    ComputationalMoleculeManager::Ptr molecule_;

    // boundary conditions, in case this is a boundary face
    BoundaryCondition::Ptr bc_;

    FluxComputationalMolecule_t computational_molecules_;
};
