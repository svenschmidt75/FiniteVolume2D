#include "MeshBuilder.h"

#include "NodeManager.h"
#include "FaceManager.h"
#include "Node.h"
#include "Face.h"
#include "EntityCreatorManager.h"
#include "Util.h"
#include "BoundaryConditionCollection.h"

#include <boost/foreach.hpp>
#include <boost/format.hpp>


MeshBuilder::MeshBuilder(EntityCreatorManager::Ptr & entity_mgr, BoundaryConditionCollection & bc) : entity_mgr_(entity_mgr), bc_(bc), mesh_(Mesh::create()) {}

bool
MeshBuilder::buildNode(IGeometricEntity::Id_t mesh_id, bool on_boundary, double x, double y) {
    NodeManager::Ptr & node_mgr = entity_mgr_->getNodeManager();
    Node::Ptr v = node_mgr->createNode(mesh_id, on_boundary, x, y);
    if (v == NULL)
        return false;
    mesh_->addNode(v);
    return true;
}

bool
MeshBuilder::buildFace(IGeometricEntity::Id_t mesh_id, bool on_boundary, std::vector<IGeometricEntity::Id_t> const & node_ids) {
    if (node_ids.size() != 2) {
        boost::format format = boost::format("MeshBuilder::buildFace: Face %1% must have 2 vertices!\n") % mesh_id;
        Util::error(format.str());
        return false;
    }

    NodeManager::Ptr & node_mgr = entity_mgr_->getNodeManager();

    EntityCollection<Node> nodes;

    BOOST_FOREACH (IGeometricEntity::Id_t node_id, node_ids) {
        Node::Ptr & v = node_mgr->getEntity(node_id);
        nodes.insert(v);
    }

    FaceManager::Ptr & face_mgr = entity_mgr_->getFaceManager();
    Face::Ptr f = face_mgr->createFace(mesh_id, on_boundary, nodes);
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

bool
MeshBuilder::buildBoundaryCondition(IGeometricEntity::Id_t face_id, BoundaryConditionCollection::Type bc_type, double bc_value) {
    return bc_.add(face_id, bc_type, bc_value);
}

boost::optional<Mesh::Ptr>
MeshBuilder::getMesh() const {
    return mesh_;
}
