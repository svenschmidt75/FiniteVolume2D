#include "VertexManager.h"

#include "Util.h"

#include <boost/format.hpp>


VertexManager::VertexManager() {}

Vertex::Ptr
VertexManager::createVertex(IGeometricEntity::Id_t mesh_id, bool on_boundary, double x, double y) {
    if (findEntity(mesh_id)) {
        boost::format format = boost::format("VertexManager::create: Vertex with mesh id %1% already created!\n") % mesh_id;
        Util::error(format.str());
        return NULL;
    }

    Vertex::Ptr vertex = Vertex::create(getNewEntityIndex(), mesh_id, on_boundary, x, y);
    collection_.insert(vertex);

    /* The vertex is referenced within the mesh file by faces
     * via the mesh_id. Hence, create a mapping between the mesh
     * id and its internal vertex id.
     */  
    mesh_id_mapping_[mesh_id] = vertex;

    return vertex;
}

VertexManager::Ptr
VertexManager::create() {
    return Ptr(new VertexManager);
}
