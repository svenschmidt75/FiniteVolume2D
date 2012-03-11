#include "EntityCreatorManager.h"

EntityCreatorManager::EntityCreatorManager(VertexManager & vertex_mgr, FaceManager & face_mgr)
    : vertex_mgr_(vertex_mgr), face_mgr_(face_mgr) {}

VertexManager &
EntityCreatorManager::getVertexManager() {
    return vertex_mgr_;
}

FaceManager &
EntityCreatorManager::getFaceManager() {
    return face_mgr_;
}
