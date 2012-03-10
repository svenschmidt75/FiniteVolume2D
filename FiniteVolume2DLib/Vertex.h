/*
 * Name  : Vertex
 * Path  : IGeometricEntity
 * Use   : Represents a 2d vertex.
 * Author: Sven Schmidt
 * Date  : 01/29/2012
 */
#pragma once

#include <memory>

#include "IGeometricEntity.h"


class Vertex : public IGeometricEntity {

    friend class VertexManager;

public:
    typedef std::shared_ptr<Vertex> Ptr;

public:
    // FROM IGeometricEntity
    IGeometricEntity::Id_t id() const;

    bool onBoundary() const;
    double x() const;
    double y() const;

private:
    Vertex(IGeometricEntity::Id_t vertex_id, double x, double y);

    // only called by VertexManager
    static Ptr create(IGeometricEntity::Id_t vertex_id, double x, double y);

private:
    IGeometricEntity::Id_t vertex_id_;
    double x_;
    double y_;
};
