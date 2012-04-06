/*
 * Name  : EntityCreatorManager
 * Path  : 
 * Use   : Contains the objects for creating mesh
 *         entities, like nodes, faces and cells.
 *         A reference is held by the mesh builder.
 * Author: Sven Schmidt
 * Date  : 03/11/2012
 */
#pragma once

#include "DeclSpec.h"

#include "NodeManager.h"
#include "FaceManager.h"
#include "CellManager.h"


#pragma warning(disable:4251)


class DECL_SYMBOLS EntityCreatorManager {
public:
    typedef std::shared_ptr<EntityCreatorManager> Ptr;

public:
    NodeManager::Ptr & getNodeManager();
    FaceManager::Ptr & getFaceManager();
    CellManager::Ptr & getCellManager();

    static EntityCreatorManager::Ptr create(NodeManager::Ptr & node_mgr, FaceManager::Ptr & face_mgr, CellManager::Ptr & cell_mgr);

private:
    EntityCreatorManager();
    EntityCreatorManager(NodeManager::Ptr & node_mgr, FaceManager::Ptr & face_mgr, CellManager::Ptr & cell_mgr);

private:
    NodeManager::Ptr node_mgr_;
    FaceManager::Ptr face_mgr_;
    CellManager::Ptr cell_mgr_;
};

#pragma warning(default:4251)
