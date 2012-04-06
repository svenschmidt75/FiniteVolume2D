/*
 * Name  : Cell
 * Path  : IGeometricEntity
 * Use   : Represents a 2d cell.
 * Author: Sven Schmidt
 * Date  : 03/17/2012
 */
#pragma once

#include <memory>

#include "DeclSpec.h"

#include "IGeometricEntity.h"
#include "EntityCollection.hpp"
#include "Node.h"
#include "Face.h"


#pragma warning(disable:4251)


class DECL_SYMBOLS Cell : public IGeometricEntity {

    friend class CellManager;

public:
    typedef std::shared_ptr<Cell> Ptr;

public:
    // FROM IGeometricEntity
    IGeometricEntity::Id_t id() const;
    IGeometricEntity::Id_t meshId() const;

    EntityCollection<Node> const & getNodes() const;
    EntityCollection<Face> const & getFaces() const;
    double                         volume() const;
    Vertex                         centroid() const;

    Vector                         faceNormal(Face::Ptr const & face) const;

private:
    Cell(IGeometricEntity::Id_t cell_id, IGeometricEntity::Id_t mesh_id, EntityCollection<Face> const & faces);

    // only called by CellManager
    static Ptr create(IGeometricEntity::Id_t cell_id, IGeometricEntity::Id_t mesh_id, EntityCollection<Face> const & faces);

private:
    IGeometricEntity::Id_t cell_id_;
    IGeometricEntity::Id_t mesh_id_;
    EntityCollection<Node> nodes_;
    EntityCollection<Face> faces_;
};

#pragma warning(default:4251)
