/*
 * Name  : VertexManager
 * Path  : 
 * Use   : Creates and holds all vertices
 * Author: Sven Schmidt
 * Date  : 03/10/2012
 */
#pragma once

#include "DeclSpec.h"

#include "EntityManager.hpp"
#include "Vertex.h"


#pragma warning(disable:4251)


class DECL_SYMBOLS VertexManager : public EntityManager<Vertex> {
public:
    Vertex::Ptr create(IGeometricEntity::Id_t vertex_id, bool on_boundary, double x, double y);
};

#pragma warning(default:4251)
