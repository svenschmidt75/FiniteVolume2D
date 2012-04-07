/*
 * Name  : MeshBuilderMock
 * Path  : 
 * Use   : Builds a mesh. Used for unit testing.
 * Author: Sven Schmidt
 * Date  : 04/01/2012
 */
#pragma once

#include "FiniteVolume2DLib/IMeshBuilder.h"
#include "FiniteVolume2DLib/NodeManager.h"
#include "FiniteVolume2DLib/FaceManager.h"
#include "FiniteVolume2DLib/CellManager.h"
#include "FiniteVolume2DLib/EntityCreatorManager.h"
#include "FiniteVolume2DLib/MeshBuilder.h"
#include "FiniteVolume2DLib/BoundaryCondition.h"


class MeshBuilderMock : public IMeshBuilder {

    friend class MeshConnectivityTest;
    friend class EntityTest;
    friend class MeshBoundaryConditionReaderTest;

public:
    MeshBuilderMock() {
        node_mgr_ = NodeManager::create();
        face_mgr_ = FaceManager::create();
        cell_mgr_ = CellManager::create();
        EntityCreatorManager::Ptr emgr = EntityCreatorManager::create(node_mgr_, face_mgr_, cell_mgr_);
        mesh_builder_ = std::shared_ptr<MeshBuilder>(new MeshBuilder(emgr, bc_));
    }

    bool buildNode(IGeometricEntity::Id_t node_id, bool on_boundary, double x, double y) {
        return mesh_builder_->buildNode(node_id, on_boundary, x, y);
    }

    bool buildFace(IGeometricEntity::Id_t face_id, bool on_boundary, std::vector<IGeometricEntity::Id_t> const & node_ids) {
        return mesh_builder_->buildFace(face_id, on_boundary, node_ids);
    }

    bool buildCell(IGeometricEntity::Id_t cell_id, std::vector<IGeometricEntity::Id_t> const & face_ids) {
        return mesh_builder_->buildCell(cell_id, face_ids);
    }

    bool buildBoundaryCondition(IGeometricEntity::Id_t face_id, BoundaryConditionCollection::Type bc_type, double bc_value) {
        return mesh_builder_->buildBoundaryCondition(face_id, bc_type, bc_value);
    }

    boost::optional<Mesh::Ptr> getMesh() const {
        return mesh_builder_->getMesh();
    }

private:
    NodeManager::Ptr  node_mgr_;
    FaceManager::Ptr  face_mgr_;
    CellManager::Ptr  cell_mgr_;
    MeshBuilder::Ptr  mesh_builder_;
    BoundaryConditionCollection bc_;
};
