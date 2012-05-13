/*
 * Name  : ComputationalFace
 * Path  : IFace
 * Use   : Computational face
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "DeclSpec.h"

#include "FiniteVolume2DLib/IFace.h"
#include "FiniteVolume2DLib/Face.h"

#include "ComputationalNode.h"
#include "BoundaryCondition.h"
#include "FluxComputationalMolecule.h"

#include <memory>


class FluxComputationalMolecule;


#pragma warning(disable:4251)
#pragma warning(disable:4275)


class DECL_SYMBOLS_2D ComputationalFace : public IFace {
public:
    typedef std::shared_ptr<ComputationalFace> Ptr;

public:
    explicit ComputationalFace(Face::Ptr const & geometric_face, EntityCollection<ComputationalNode> const & cnodes);

    // FROM IGeometricEntity
    IGeometricEntity::Id_t id() const;
    IGeometricEntity::Id_t meshId() const;

    // FROM IFace
    IGeometricEntity::Entity_t     getEntityType() const;
    EntityCollection<Node> const & getNodes() const;
    double                         area() const;
    Vector                         normal() const;
    Vertex                         centroid() const;


    EntityCollection<ComputationalNode> const & getComputationalNodes() const;

    BoundaryCondition const &                   getBoundaryCondition() const;
    void                                        setBoundaryCondition(BoundaryCondition::Ptr const & bc);

    FluxComputationalMolecule &                 getComputationalMolecule(std::string const & name);
    void                                        addComputationalMolecule(std::string const & name);

private:
    typedef std::map<std::string, FluxComputationalMolecule> FluxComputationalMoleculeManager_t;

private:
    // the geometric partner face
    Face::Ptr geometric_face_;

    EntityCollection<ComputationalNode> cnodes_;

    /* A computational face may have a set of molecules,
     * but does not have to.
     */
    FluxComputationalMoleculeManager_t  cm_;

    // boundary conditions, in case this is a boundary face
    BoundaryCondition::Ptr              bc_;
};

#pragma warning(default:4275)
#pragma warning(default:4251)
