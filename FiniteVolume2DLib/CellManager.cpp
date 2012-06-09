#include "CellManager.h"

#include "Util.h"

#include <boost/format.hpp>


CellManager::CellManager() {}

Cell::Ptr
CellManager::createCell(IGeometricEntity::Id_t mesh_id, EntityCollection<Face const> const & faces) {
    if (findEntity(mesh_id)) {
        boost::format format = boost::format("CellManager::create: Cell with mesh id %1% already created!\n") % mesh_id;
        Util::error(format.str());
        return NULL;
    }

    Cell::Ptr cell = Cell::create(getNewEntityIndex(), mesh_id, faces);
    collection_.insert(cell);

    /* The cell is referenced within the mesh file via the
     * mesh_id. Hence, create a mapping between the mesh
     * id and its internal cell id.
     */  
    mesh_id_mapping_[mesh_id] = cell;

    return cell;
}

CellManager::Ptr
CellManager::create() {
    return Ptr(new CellManager);
}
