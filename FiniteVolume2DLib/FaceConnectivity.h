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
    void                                          insert(Cell::CPtr const & cell);
    boost::optional<EntityCollection<Cell const>> getCellsAttachedToFace(Face::CPtr const & face) const;
    Cell::CPtr                                    getOtherCell(Face::CPtr const & face, Cell::CPtr const & cell) const;

private:
    typedef std::map<IGeometricEntity::Id_t, EntityCollection<Cell const>> FaceCells_t;
    typedef std::map<IGeometricEntity::Id_t, EntityCollection<Cell const>>::const_iterator const_iterator;

private:
    // attached cells
    FaceCells_t face_cells_;
};
