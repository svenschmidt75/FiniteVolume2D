#include "EntityCreatorManager.h"


EntityCreatorManager::EntityCreatorManager(NodeManager::Ptr & node_mgr, FaceManager::Ptr & face_mgr, CellManager::Ptr & cell_mgr)
    : node_mgr_(node_mgr), face_mgr_(face_mgr), cell_mgr_(cell_mgr) {}

NodeManager::Ptr &
EntityCreatorManager::getNodeManager() {
    return node_mgr_;
}

FaceManager::Ptr &
EntityCreatorManager::getFaceManager() {
    return face_mgr_;
}

CellManager::Ptr &
EntityCreatorManager::getCellManager() {
    return cell_mgr_;
}

EntityCreatorManager::Ptr
EntityCreatorManager::create(NodeManager::Ptr & node_mgr, FaceManager::Ptr & face_mgr, CellManager::Ptr & cell_mgr) {
    return Ptr(new EntityCreatorManager(node_mgr, face_mgr, cell_mgr));
}
