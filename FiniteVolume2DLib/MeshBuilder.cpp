#include "MeshBuilder.h"

#include "VertexManager.h"
#include "FaceManager.h"
#include "Vertex.h"
#include "Face.h"
#include "EntityCreatorManager.h"

#include <boost/foreach.hpp>


MeshBuilder::MeshBuilder(EntityCreatorManager & entity_mgr) : entity_mgr_(entity_mgr) {}

bool
MeshBuilder::buildVertex(IGeometricEntity::Id_t mesh_id, bool on_boundary, double x, double y) {
    VertexManager & vertex_mgr = entity_mgr_.getVertexManager();
    Vertex::Ptr v = vertex_mgr.create(mesh_id, on_boundary, x, y);
    if (v == NULL)
        return false;
    mesh_->addVertex(v);
    return true;
}

bool
MeshBuilder::buildFace(IGeometricEntity::Id_t mesh_id, bool on_boundary, std::vector<IGeometricEntity::Id_t> const & vertex_ids) {
    VertexManager & vertex_mgr = entity_mgr_.getVertexManager();

    EntityCollection<Vertex> vertices;

    BOOST_FOREACH (IGeometricEntity::Id_t vertex_id, vertex_ids) {
        Vertex::Ptr v = vertex_mgr.getVertex(vertex_id);
        vertices.insert(v);
    }

    FaceManager & face_mgr = entity_mgr_.getFaceManager();
    Face::Ptr f = face_mgr.create(mesh_id, on_boundary, vertices);
    if (f == NULL)
        return false;
    mesh_->addFace(f);
    return true;
}

bool
MeshBuilder::buildCell(IGeometricEntity::Id_t cell_id, std::vector<IGeometricEntity::Id_t> const & face_ids) {
    return false;
}

boost::optional<Mesh::Ptr>
MeshBuilder::getMesh() const {
    return mesh_;
}
