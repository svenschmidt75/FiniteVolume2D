/*
 * Name  : ComputationalCell
 * Path  : ICell
 * Use   : Computational cell
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "DeclSpec.h"

#include "ComputationalNode.h"
#include "ComputationalFace.h"
#include "ComputationalVariable.h"

#include "FiniteVolume2DLib/ICell.h"
#include "FiniteVolume2DLib/Cell.h"
#include "FiniteVolume2DLib/Face.h"

#include "FiniteVolume2D/ComputationalMolecule.h"

#include <memory>
#include <map>


#pragma warning(disable:4251)
#pragma warning(disable:4275)


class DECL_SYMBOLS_2D ComputationalCell : public ICell {
public:
    typedef std::shared_ptr<ComputationalCell>       Ptr;

public:
    explicit ComputationalCell(Cell::Ptr const & geometric_cell, EntityCollection<ComputationalFace> const & faces);

    // FROM IGeometricEntity
    IGeometricEntity::Id_t id() const;
    IGeometricEntity::Id_t meshId() const;

    // FROM ICell
    EntityCollection<Node> const & getNodes() const;
    EntityCollection<Face> const & getFaces() const;
    double                         volume() const;
    Vertex                         centroid() const;

    Vector                         faceNormal(Face::Ptr const & face) const;

    Cell::Ptr const &              geometricEntity() const;


    EntityCollection<ComputationalNode> const & getComputationalNodes() const;
    EntityCollection<ComputationalFace> const & getComputationalFaces() const;

    ComputationalVariable::Ptr const getComputationalVariable(std::string const & name) const;
    void                              addComputationalVariable(ComputationalVariable::Ptr const & cvar);

    ComputationalMolecule &           getComputationalMolecule(std::string const & name);
    void                              addComputationalMolecule(std::string const & name);

private:
    typedef std::map<std::string, ComputationalVariable::Ptr> ComputationalVariables_t;
    typedef std::map<std::string, ComputationalMolecule> ComputationalMolecules_t;

private:
    // the geometric partner cell
    Cell::Ptr                           geometric_cell_;

    EntityCollection<ComputationalNode> nodes_;
    EntityCollection<ComputationalFace> faces_;

    // all variables that will be solved for
    ComputationalVariables_t            cvars_;

    /* All computational molecules for this cell,
     * for both active and passive variables.
     */
    ComputationalMolecules_t            cm_;
};

#pragma warning(default:4275)
#pragma warning(default:4251)
