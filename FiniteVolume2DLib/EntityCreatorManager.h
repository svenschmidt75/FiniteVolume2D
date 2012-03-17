/*
 * Name  : EntityCreatorManager
 * Path  : 
 * Use   : Contains the objects for creating mesh
 *         entities, like vertices, faces and cells.
 *         A reference is held by the mesh builder.
 * Author: Sven Schmidt
 * Date  : 03/11/2012
 */
#pragma once

class VertexManager;
class FaceManager;
class CellManager;


class EntityCreatorManager {
public:
    EntityCreatorManager(VertexManager & vertex_mgr, FaceManager & face_mgr, CellManager & cell_mgr);

    VertexManager & getVertexManager();
    FaceManager   & getFaceManager();
    CellManager   & getCellManager();

private:
    VertexManager & vertex_mgr_;
    FaceManager   & face_mgr_;
    CellManager   & cell_mgr_;
};
