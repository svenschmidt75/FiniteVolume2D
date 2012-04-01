/*
 * Name  : Face
 * Path  : IGeometricEntity
 * Use   : Represents a 2d face.
 * Author: Sven Schmidt
 * Date  : 03/11/2012
 */
#pragma once

#include <memory>

#include "DeclSpec.h"

#include "IGeometricEntity.h"
#include "EntityCollection.hpp"
#include "Vertex.h"

class Vector;


#pragma warning(disable:4251)


class DECL_SYMBOLS Face : public IGeometricEntity {

    friend class FaceManager;

public:
    typedef std::shared_ptr<Face> Ptr;

public:
    // FROM IGeometricEntity
    IGeometricEntity::Id_t id() const;
    IGeometricEntity::Id_t meshId() const;

    bool                             onBoundary() const;
    EntityCollection<Vertex> const & getVertices() const;
    double                           area() const;
    Vector                           normal() const;
    Vertex                           centroid() const;

private:
    Face(IGeometricEntity::Id_t face_id, IGeometricEntity::Id_t mesh_id, bool on_boundary, EntityCollection<Vertex> const & vertices);

    // only called by FaceManager
    static Ptr create(IGeometricEntity::Id_t face_id, IGeometricEntity::Id_t mesh_id, bool on_boundary, EntityCollection<Vertex> const & vertices);

private:
    IGeometricEntity::Id_t   face_id_;
    IGeometricEntity::Id_t   mesh_id_;
    bool                     on_boundary_;
    EntityCollection<Vertex> vertices_;
};

#pragma warning(default:4251)
