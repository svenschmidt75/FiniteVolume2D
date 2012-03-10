#include "VertexManager.h"

Vertex::Ptr
VertexManager::create(IGeometricEntity::Id_t mesh_id, double x, double y) {
    Vertex::Ptr vertex = Vertex::create(getNewEntityIndex(), x, y);
    collection_.insert(vertex);

    /* The vertex is references within the mesh file by faces
     * via the mesh_id. Hence, create a mapping between the mesh
     * id and its internal vertex id.
     */
    mesh_id_mapping_[mesh_id] = vertex;

    return vertex;
}
