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


class DECL_SYMBOLS_2DLIB FaceManager : public EntityManager<Face> {
public:
    typedef std::shared_ptr<FaceManager> Ptr;

public:
    Face::Ptr createFace(IGeometricEntity::Id_t mesh_id, IGeometricEntity::Entity_t entity_type, EntityCollection<Node const> const & nodes);

    static Ptr create();

private:
    FaceManager();
};

#pragma warning(default:4251)
