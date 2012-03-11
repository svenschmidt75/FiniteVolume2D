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
    Face::Ptr create(IGeometricEntity::Id_t face_id, bool on_boundary, EntityCollection<Vertex> const & vertices);
};

#pragma warning(default:4251)
