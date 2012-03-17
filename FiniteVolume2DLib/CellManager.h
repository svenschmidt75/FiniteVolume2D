/*
 * Name  : CellManager
 * Path  : 
 * Use   : Creates and holds all cells
 * Author: Sven Schmidt
 * Date  : 03/17/2012
 */
#pragma once

#include "DeclSpec.h"

#include "EntityManager.hpp"
#include "Cell.h"


#pragma warning(disable:4251)


class DECL_SYMBOLS CellManager : public EntityManager<Cell> {
public:
    Cell::Ptr create(IGeometricEntity::Id_t mesh_id, EntityCollection<Face> const & faces);
};

#pragma warning(default:4251)
