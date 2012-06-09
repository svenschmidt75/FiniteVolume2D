/*
 * Name  : Face
 * Path  : IFace
 * Use   : Represents a 2d face.
 * Author: Sven Schmidt
 * Date  : 03/11/2012
 */
#pragma once

#include <memory>

#include "DeclSpec.h"

#include "IFace.h"
#include "EntityCollection.hpp"
#include "Node.h"


#pragma warning(disable:4251)
#pragma warning(disable:4275)


class DECL_SYMBOLS_2DLIB Face : public IFace {

    friend class FaceManager;

public:
    typedef std::shared_ptr<Face>       Ptr;
    typedef std::shared_ptr<Face const> CPtr;

public:
    // FROM IGeometricEntity
    IGeometricEntity::Id_t id() const;
    IGeometricEntity::Id_t meshId() const;

    // FROM IFace
    IGeometricEntity::Entity_t     getEntityType() const;
    EntityCollection<Node> const & getNodes() const;
    double                         area() const;
    Vector                         normal() const;
    Vertex                         centroid() const;

private:
    Face(IGeometricEntity::Id_t face_id, IGeometricEntity::Id_t mesh_id, IGeometricEntity::Entity_t entity_type, EntityCollection<Node> const & vertices);

    // only called by FaceManager
    static Ptr create(IGeometricEntity::Id_t face_id, IGeometricEntity::Id_t mesh_id, IGeometricEntity::Entity_t entity_type, EntityCollection<Node> const & nodes);

private:
    IGeometricEntity::Id_t     face_id_;
    IGeometricEntity::Id_t     mesh_id_;
    IGeometricEntity::Entity_t entity_type_;
    EntityCollection<Node>     nodes_;
};

#pragma warning(default:4275)
#pragma warning(default:4251)
