/*
 * Name  : MeshBuilder
 * Path  : IMeshBuilder
 * Use   : Builds a mesh
 * Author: Sven Schmidt
 * Date  : 03/03/2012
 */
#pragma once

#include "IMeshBuilder.h"

class Mesh;


class MeshBuilder : public IMeshBuilder {
public:
    // FROM IMshBuilder
    bool buildVertex(IGeometricEntity::Id_t vertex_id, bool on_boundary, double x, double y);
    bool buildFace(IGeometricEntity::Id_t face_id, bool on_boundary, std::vector<IGeometricEntity::Id_t> const & vertex_ids);
    bool buildCell(IGeometricEntity::Id_t cell_id, std::vector<IGeometricEntity::Id_t> const & face_ids);

    boost::optional<Mesh::Ptr> getMesh() const;
};
