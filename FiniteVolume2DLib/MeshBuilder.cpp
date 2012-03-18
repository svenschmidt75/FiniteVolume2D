#include "MeshBuilder.h"

#include "VertexManager.h"
#include "FaceManager.h"
#include "Vertex.h"
#include "Face.h"
#include "EntityCreatorManager.h"
#include "Util.h"

#include <boost/foreach.hpp>
#include <boost/format.hpp>


MeshBuilder::MeshBuilder(EntityCreatorManager::Ptr & entity_mgr) : entity_mgr_(entity_mgr), mesh_(Mesh::create()) {}

bool
MeshBuilder::buildVertex(IGeometricEntity::Id_t mesh_id, bool on_boundary, double x, double y) {
    VertexManager::Ptr & vertex_mgr = entity_mgr_->getVertexManager();
    Vertex::Ptr v = vertex_mgr->createVertex(mesh_id, on_boundary, x, y);
    if (v == NULL)
        return false;
    mesh_->addVertex(v);
    return true;
}

bool
MeshBuilder::buildFace(IGeometricEntity::Id_t mesh_id, bool on_boundary, std::vector<IGeometricEntity::Id_t> const & vertex_ids) {
    if (vertex_ids.size() != 2) {
        boost::format format = boost::format("MeshBuilder::buildFace: Face %1% must have 2 vertices!\n") % mesh_id;
        Util::error(format.str());
        return false;
    }

    VertexManager::Ptr & vertex_mgr = entity_mgr_->getVertexManager();

    EntityCollection<Vertex> vertices;

    BOOST_FOREACH (IGeometricEntity::Id_t vertex_id, vertex_ids) {
        Vertex::Ptr & v = vertex_mgr->getEntity(vertex_id);
        vertices.insert(v);
    }

    FaceManager::Ptr & face_mgr = entity_mgr_->getFaceManager();
    Face::Ptr f = face_mgr->createFace(mesh_id, on_boundary, vertices);
    if (f == NULL)
        return false;
    mesh_->addFace(f);
    return true;
}

bool
MeshBuilder::buildCell(IGeometricEntity::Id_t mesh_id, std::vector<IGeometricEntity::Id_t> const & face_ids) {
    FaceManager::Ptr & face_mgr = entity_mgr_->getFaceManager();

    EntityCollection<Face> faces;

    BOOST_FOREACH (IGeometricEntity::Id_t face_id, face_ids) {
        Face::Ptr & f = face_mgr->getEntity(face_id);
        faces.insert(f);
    }

    CellManager::Ptr & cell_mgr = entity_mgr_->getCellManager();
    Cell::Ptr c = cell_mgr->createCell(mesh_id, faces);
    if (c == NULL)
        return false;
    mesh_->addCell(c);
    return true;
}

boost::optional<Mesh::Ptr>
MeshBuilder::getMesh() const {
    return mesh_;
}
