/*
 * Name  : FaceManager
 * Path  : 
 * Use   : Creates faces
 * Author: Sven Schmidt
 * Date  : 03/11/2012
 */
#pragma once

#include "DeclSpec.h"

#include "EntityManager.hpp"
#include "Face.h"


#pragma warning(disable:4251)


class DECL_SYMBOLS FaceManager : public EntityManager<Face> {
public:
    typedef std::shared_ptr<FaceManager> Ptr;

public:
    Face::Ptr createFace(IGeometricEntity::Id_t mesh_id, bool on_boundary, EntityCollection<Vertex> const & vertices);

    static Ptr create();

private:
    FaceManager();
};

#pragma warning(default:4251)
