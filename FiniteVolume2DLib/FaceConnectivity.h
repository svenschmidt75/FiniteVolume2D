/*
 * Name  : FaceConnectivity
 * Path  : 
 * Use   : Face connectivity
 * Author: Sven Schmidt
 * Date  : 03/17/2012
 */
#pragma once

#include "Face.h"
#include "Cell.h"

#include <map>

#include <boost/optional.hpp>


class FaceConnectivity {
public:
    void                                      insert(Cell::Ptr const & cell);
    boost::optional<EntityCollection<Cell>>   getCellsAttachedToFace(Face::Ptr const & face) const;
    Cell::Ptr                                 getOtherCell(Face::Ptr const & face, Cell::Ptr const & cell) const;

private:
    typedef std::map<IGeometricEntity::Id_t, EntityCollection<Cell>> FaceCells_t;
    typedef std::map<IGeometricEntity::Id_t, EntityCollection<Cell>>::const_iterator const_iterator;

private:
    // attached cells
    FaceCells_t face_cells_;
};
