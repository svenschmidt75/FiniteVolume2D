/*
 * Name  : MeshBuilder
 * Path  : IMeshBuilder
 * Use   : Builds a mesh
 * Author: Sven Schmidt
 * Date  : 03/03/2012
 */
#pragma once

#include "IMeshBuilder.h"
#include "Mesh.h"

class EntityCreatorManager;


class MeshBuilder : public IMeshBuilder {
public:
    MeshBuilder(EntityCreatorManager & entity_mgr);

    // FROM IMeshBuilder
    bool buildVertex(IGeometricEntity::Id_t mesh_id, bool on_boundary, double x, double y);
    bool buildFace(IGeometricEntity::Id_t mesh_id, bool on_boundary, std::vector<IGeometricEntity::Id_t> const & vertex_ids);
    bool buildCell(IGeometricEntity::Id_t mesh_id, std::vector<IGeometricEntity::Id_t> const & face_ids);

    boost::optional<Mesh::Ptr> getMesh() const;

private:
    Mesh::Ptr mesh_;

    EntityCreatorManager & entity_mgr_;
};
