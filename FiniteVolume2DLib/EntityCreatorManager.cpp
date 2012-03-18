#include "EntityCreatorManager.h"

EntityCreatorManager::EntityCreatorManager(VertexManager::Ptr & vertex_mgr, FaceManager::Ptr & face_mgr, CellManager::Ptr & cell_mgr)
    : vertex_mgr_(vertex_mgr), face_mgr_(face_mgr), cell_mgr_(cell_mgr) {}

VertexManager::Ptr &
EntityCreatorManager::getVertexManager() {
    return vertex_mgr_;
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
EntityCreatorManager::create(VertexManager::Ptr & vertex_mgr, FaceManager::Ptr & face_mgr, CellManager::Ptr & cell_mgr) {
    return Ptr(new EntityCreatorManager(vertex_mgr, face_mgr, cell_mgr));
}
