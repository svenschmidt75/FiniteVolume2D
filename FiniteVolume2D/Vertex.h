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
    typedef std::unique_ptr<Vertex> Ptr;

public:
    double x() const;
    double y() const;

private:
    Vertex(IGeometricEntity::Id_t id);

    // only called by VertexManager
    static Ptr create();

private:
    double x_;
    double y_;
};
