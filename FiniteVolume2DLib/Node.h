/*
 * Name  : Node
 * Path  : INode
 * Use   : Represents a 2d node, i.e. a controlled (managed)
 *         2d vertex.
 * Author: Sven Schmidt
 * Date  : 01/29/2012
 */
#pragma once

#include <memory>

#include "DeclSpec.h"

#include "INode.h"
#include "Vertex.h"

class Vector;

#pragma warning(disable:4251)
#pragma warning(disable:4275)


class DECL_SYMBOLS_2DLIB Node : public INode {

    friend class NodeManager;

public:
    typedef std::shared_ptr<Node> Ptr;

public:
    // FROM IGeometricEntity
    IGeometricEntity::Id_t id() const;
    IGeometricEntity::Id_t meshId() const;

    // FROM INode
    IGeometricEntity::Entity_t getEntityType() const;
    Vertex                     location() const;

private:
    Node(IGeometricEntity::Id_t node_id, IGeometricEntity::Id_t mesh_id, IGeometricEntity::Entity_t entity_type, double x, double y);

    // only called by VertexManager
    static Ptr create(IGeometricEntity::Id_t node_id, IGeometricEntity::Id_t mesh_id, IGeometricEntity::Entity_t entity_type, double x, double y);

private:
    IGeometricEntity::Id_t     node_id_;
    IGeometricEntity::Id_t     mesh_id_;
    IGeometricEntity::Entity_t entity_type_;
    Vertex                     vertex_;
};

#pragma warning(default:4275)
#pragma warning(default:4251)
