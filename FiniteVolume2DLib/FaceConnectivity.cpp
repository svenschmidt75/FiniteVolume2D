#include "FaceConnectivity.h"

#include "Util.h"

#include <boost/format.hpp>


void
FaceConnectivity::insert(Cell::Ptr const & cell) {
    // attach this cell to all its faces
    typedef EntityCollection<Face>::size_type size_type;

    EntityCollection<Face> const & faces = cell->getFaces();
    for (size_type i = 0; i < faces.size(); ++i) {
        Face::Ptr const & face = faces.getEntity(i);
        face_cells_[face->id()].insert(cell);
    }
}

boost::optional<EntityCollection<Cell>>
FaceConnectivity::getCellsAttachedToFace(Face::Ptr const & face) const {
    const_iterator it = face_cells_.find(face->id());
    if (it == face_cells_.end())
        return boost::optional<EntityCollection<Cell>>();

    return boost::optional<EntityCollection<Cell>>(it->second);
}

Cell::Ptr
FaceConnectivity::getOtherCell(Face::Ptr const & face, Cell::Ptr const & cell) const {
    const_iterator it = face_cells_.find(face->id());
    if (it == face_cells_.end()) {
        // face not found
        assert(false);
        return nullptr;
    }

    EntityCollection<Cell> const & cells = it->second;

    // interior face
    if (face->getEntityType() == IGeometricEntity::INTERIOR) {
        if (cells.size() != 2) {
            boost::format format = boost::format("FaceConnectivity::getOtherCell: Interior face %1% must have 2 cells!\n") % face->meshId();
            Util::error(format.str());
            return nullptr;

        }
    }
    else {
        // boundary face
        if (cells.size() != 1) {
            boost::format format = boost::format("FaceConnectivity::getOtherCell: Boundary face %1% must have 1 cell!\n") % face->meshId();
            Util::error(format.str());
            return nullptr;
        }
    }

    if (!cells.find(cell->meshId())) {
        boost::format format = boost::format("FaceConnectivity::getOtherCell: Face %1% does not reference cell %2%!\n") % face->meshId() % cell->meshId();
        Util::error(format.str());
        return nullptr;
    }

    if (cells.size() == 1)
        return nullptr;

    if (cells.getEntity(0) == cell)
        return cells.getEntity(1);
    return cells.getEntity(0);
}
