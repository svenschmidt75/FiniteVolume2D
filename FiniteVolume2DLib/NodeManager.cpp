#include "NodeManager.h"

#include "Util.h"

#include <boost/format.hpp>


NodeManager::NodeManager() {}

Node::Ptr NodeManager::createNode(IGeometricEntity::Id_t mesh_id, IGeometricEntity::Entity_t entity_type, double x, double y)
{
    if (findEntity(mesh_id)) {
        boost::format format = boost::format("VertexManager::create: Vertex with mesh id %1% already created!\n") % mesh_id;
        Util::error(format.str());
        return NULL;
    }

    Node::Ptr node = Node::create(getNewEntityIndex(), mesh_id, entity_type, x, y);
    collection_.insert(node);

    /* The node is referenced within the mesh file by faces
     * via the mesh_id. Hence, create a mapping between the mesh
     * id and its internal node id.
     */  
    mesh_id_mapping_[mesh_id] = node;

    return node;
}

NodeManager::Ptr
NodeManager::create() {
    return Ptr(new NodeManager);
}
