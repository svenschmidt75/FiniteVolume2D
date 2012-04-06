#include "FaceManager.h"

#include "Util.h"

#include <boost/format.hpp>


FaceManager::FaceManager() {}

Face::Ptr
FaceManager::createFace(IGeometricEntity::Id_t mesh_id, bool on_boundary, EntityCollection<Node> const & vertices) {
    if (findEntity(mesh_id)) {
        boost::format format = boost::format("FaceManager::create: Face with mesh id %1% already created!\n") % mesh_id;
        Util::error(format.str());
        return NULL;
    }

    Face::Ptr face = Face::create(getNewEntityIndex(), mesh_id, on_boundary, vertices);
    collection_.insert(face);

    /* The face is referenced within the mesh file by faces
     * via the mesh_id. Hence, create a mapping between the mesh
     * id and its internal face id.
     */  
    mesh_id_mapping_[mesh_id] = face;

    return face;
}

FaceManager::Ptr
FaceManager::create() {
    return Ptr(new FaceManager);
}
