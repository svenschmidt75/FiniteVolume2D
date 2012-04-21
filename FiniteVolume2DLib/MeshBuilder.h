/*
 * Name  : MeshBuilder
 * Path  : IMeshBuilder
 * Use   : Builds a mesh
 * Author: Sven Schmidt
 * Date  : 03/03/2012
 */
#pragma once

#include "DeclSpec.h"

#include "IMeshBuilder.h"
#include "Mesh.h"
#include "EntityCreatorManager.h"


#pragma warning(disable:4251)


class DECL_SYMBOLS MeshBuilder : public IMeshBuilder {
public:
    typedef std::shared_ptr<MeshBuilder> Ptr;

public:
    MeshBuilder(EntityCreatorManager::Ptr & entity_mgr, BoundaryConditionCollection & bc);


    // FROM IMeshBuilder
    bool buildNode(IGeometricEntity::Id_t mesh_id, IGeometricEntity::Entity_t entity_type, double x, double y);
    bool buildFace(IGeometricEntity::Id_t mesh_id, IGeometricEntity::Entity_t entity_type, std::vector<IGeometricEntity::Id_t> const & node_ids);
    bool buildCell(IGeometricEntity::Id_t mesh_id, std::vector<IGeometricEntity::Id_t> const & face_ids);

    boost::optional<Mesh::Ptr> getMesh() const;

private:
    MeshBuilder(MeshBuilder const & in);
    MeshBuilder & operator=(MeshBuilder const & in);

private:
    Mesh::Ptr                 mesh_;
    EntityCreatorManager::Ptr entity_mgr_;
    BoundaryConditionCollection &       bc_;
};

#pragma warning(default:4251)
