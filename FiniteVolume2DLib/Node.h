/*
 * Name  : Vertex
 * Path  : IGeometricEntity
 * Use   : Represents a 2d node, i.e. a controlled (managed)
 *         2d vertex.
 * Author: Sven Schmidt
 * Date  : 01/29/2012
 */
#pragma once

#include <memory>

#include "DeclSpec.h"

#include "IGeometricEntity.h"
#include "Vertex.h"

class Vector;

#pragma warning(disable:4251)


class DECL_SYMBOLS Node : public IGeometricEntity {

    friend class NodeManager;

public:
    typedef std::shared_ptr<Node> Ptr;

public:
    // FROM IGeometricEntity
    IGeometricEntity::Id_t id() const;
    IGeometricEntity::Id_t meshId() const;

    bool   onBoundary() const;
    double x() const;
    double y() const;
    Vector vector() const;

private:
    Node(IGeometricEntity::Id_t node_id, IGeometricEntity::Id_t mesh_id, bool on_boundary, double x, double y);

    // only called by VertexManager
    static Ptr create(IGeometricEntity::Id_t node_id, IGeometricEntity::Id_t mesh_id, bool on_boundary, double x, double y);

private:
    IGeometricEntity::Id_t node_id_;
    IGeometricEntity::Id_t mesh_id_;
    bool                   on_boundary_;
    Vertex                 vertex_;
};

#pragma warning(default:4251)
