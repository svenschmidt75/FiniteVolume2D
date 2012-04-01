/*
 * Name  : Vertex
 * Path  : IGeometricEntity
 * Use   : Represents a 2d vertex.
 * Author: Sven Schmidt
 * Date  : 01/29/2012
 */
#pragma once

#include <memory>

#include "DeclSpec.h"

#include "IGeometricEntity.h"

class Vector;

#pragma warning(disable:4251)


class DECL_SYMBOLS Vertex : public IGeometricEntity {

    friend class VertexManager;

public:
    typedef std::shared_ptr<Vertex> Ptr;

public:
    // FROM IGeometricEntity
    IGeometricEntity::Id_t id() const;
    IGeometricEntity::Id_t meshId() const;

    bool   onBoundary() const;
    double x() const;
    double y() const;
    Vector vector() const;

private:
    Vertex(IGeometricEntity::Id_t vertex_id, IGeometricEntity::Id_t mesh_id, bool on_boundary, double x, double y);

    // only called by VertexManager
    static Ptr create(IGeometricEntity::Id_t vertex_id, IGeometricEntity::Id_t mesh_id, bool on_boundary, double x, double y);

private:
    IGeometricEntity::Id_t vertex_id_;
    IGeometricEntity::Id_t mesh_id_;
    bool                   on_boundary_;
    double                 x_;
    double                 y_;
};

#pragma warning(default:4251)
