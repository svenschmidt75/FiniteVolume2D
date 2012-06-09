/*
 * Name  : Cell
 * Path  : ICell
 * Use   : Represents a 2d cell.
 * Author: Sven Schmidt
 * Date  : 03/17/2012
 */
#pragma once

#include <memory>

#include "DeclSpec.h"

#include "ICell.h"
#include "EntityCollection.hpp"
#include "Node.h"
#include "Face.h"


#pragma warning(disable:4251)
#pragma warning(disable:4275)


class DECL_SYMBOLS_2DLIB Cell : public ICell {

    friend class CellManager;

public:
    typedef std::shared_ptr<Cell>       Ptr;
    typedef std::shared_ptr<Cell const> CPtr;

public:
    // FROM IGeometricEntity
    IGeometricEntity::Id_t id() const;
    IGeometricEntity::Id_t meshId() const;

    // FROM ICell
    EntityCollection<Node const> const & getNodes() const;
    EntityCollection<Face const> const & getFaces() const;
    double                               volume() const;
    Vertex                               centroid() const;

    Vector                               faceNormal(Face::CPtr const & face) const;

private:
    Cell(IGeometricEntity::Id_t cell_id, IGeometricEntity::Id_t mesh_id, EntityCollection<Face const> const & faces);

    // only called by CellManager
    static Ptr create(IGeometricEntity::Id_t cell_id, IGeometricEntity::Id_t mesh_id, EntityCollection<Face const> const & faces);

private:
    IGeometricEntity::Id_t       cell_id_;
    IGeometricEntity::Id_t       mesh_id_;
    EntityCollection<Node const> nodes_;
    EntityCollection<Face const> faces_;
};

#pragma warning(default:4275)
#pragma warning(default:4251)
