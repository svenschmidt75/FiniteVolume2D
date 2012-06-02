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
#include "FiniteVolume2DLib/BoundaryConditionCollection.h"


class MeshBuilderMock : public IMeshBuilder {

    friend class MeshConnectivityTest;
    friend class EntityTest;
    friend class MeshBoundaryConditionReaderTest;
    friend class ComputationalVariableTest;
    friend class ComputationalMeshBuilderTest;

public:
    MeshBuilderMock() {
        node_mgr_ = NodeManager::create();
        face_mgr_ = FaceManager::create();
        cell_mgr_ = CellManager::create();
        EntityCreatorManager::Ptr emgr = EntityCreatorManager::create(node_mgr_, face_mgr_, cell_mgr_);
        mesh_builder_ = std::make_shared<MeshBuilder>(emgr, bc_);
    }

    bool buildNode(IGeometricEntity::Id_t node_id, IGeometricEntity::Entity_t entity_type, double x, double y) {
        return mesh_builder_->buildNode(node_id, entity_type, x, y);
    }

    bool buildFace(IGeometricEntity::Id_t face_id, IGeometricEntity::Entity_t entity_type, std::vector<IGeometricEntity::Id_t> const & node_ids) {
        return mesh_builder_->buildFace(face_id, entity_type, node_ids);
    }

    bool buildCell(IGeometricEntity::Id_t cell_id, std::vector<IGeometricEntity::Id_t> const & face_ids) {
        return mesh_builder_->buildCell(cell_id, face_ids);
    }

    boost::optional<Mesh::Ptr> getMesh() const {
        return mesh_builder_->getMesh();
    }

private:
    NodeManager::Ptr            node_mgr_;
    FaceManager::Ptr            face_mgr_;
    CellManager::Ptr            cell_mgr_;
    MeshBuilder::Ptr            mesh_builder_;
    BoundaryConditionCollection bc_;
};
