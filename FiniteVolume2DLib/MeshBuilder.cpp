#include "MeshBuilder.h"

#include "VertexManager.h"
#include "Vertex.h"


bool
MeshBuilder::buildVertex(IGeometricEntity::Id_t mesh_id, bool on_boundary, double x, double y) {
    Vertex::Ptr v = VertexManager::create(mesh_id, x, y);
    mesh_->addVertex(v, on_boundary);
}

bool
MeshBuilder::buildFace(IGeometricEntity::Id_t face_id, bool on_boundary, std::vector<IGeometricEntity::Id_t> const & vertex_ids) {

}

bool
MeshBuilder::buildCell(IGeometricEntity::Id_t cell_id, std::vector<IGeometricEntity::Id_t> const & face_ids) {

}

boost::optional<Mesh::Ptr>
MeshBuilder::getMesh() const {
    return mesh_;
}
