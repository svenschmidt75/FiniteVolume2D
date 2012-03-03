/*
 * Name  : MeshBuilder
 * Path  : 
 * Use   : Builds a mesh
 * Author: Sven Schmidt
 * Date  : 03/03/2012
 */
#pragma once

class Mesh;


class MeshBuilder {
public:
    bool buildVertex(IGeometricEntity::Id_t vertex_id, double x, double y);
    bool buildFace(IGeometricEntity::Id_t face_id, std::vector<IGeometricEntity::Id_t> const & vertex_ids);
    bool buildCell(IGeometricEntity::Id_t cell_id, std::vector<IGeometricEntity::Id_t> const & face_ids);

    Mesh const & getMesh() const;
};
