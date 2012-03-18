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
    typedef std::shared_ptr<VertexManager> Ptr;

public:
    Vertex::Ptr createVertex(IGeometricEntity::Id_t mesh_id, bool on_boundary, double x, double y);

    static Ptr create();

private:
    VertexManager();
};

#pragma warning(default:4251)
