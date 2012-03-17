/*
 * Name  : Cell
 * Path  : IGeometricEntity
 * Use   : Represents a 2d cell.
 * Author: Sven Schmidt
 * Date  : 03/17/2012
 */
#pragma once

#include <memory>

#include "IGeometricEntity.h"
#include "EntityCollection.hpp"
#include "Vertex.h"
#include "Face.h"


class Cell : public IGeometricEntity {

    friend class CellManager;

public:
    typedef std::shared_ptr<Cell> Ptr;

public:
    // FROM IGeometricEntity
    IGeometricEntity::Id_t id() const;
    IGeometricEntity::Id_t meshId() const;

    EntityCollection<Vertex> const & getVertices() const;
    EntityCollection<Face> const &   getFaces() const;

private:
    Cell(IGeometricEntity::Id_t cell_id, IGeometricEntity::Id_t mesh_id, EntityCollection<Face> const & faces);

    // only called by CellManager
    static Ptr create(IGeometricEntity::Id_t cell_id, IGeometricEntity::Id_t mesh_id, EntityCollection<Face> const & faces);

private:
    IGeometricEntity::Id_t cell_id_;
    IGeometricEntity::Id_t mesh_id_;
    EntityCollection<Vertex> vertices_;
    EntityCollection<Face> faces_;
};
