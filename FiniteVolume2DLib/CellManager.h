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


class DECL_SYMBOLS_2DLIB CellManager : public EntityManager<Cell> {
public:
    typedef std::shared_ptr<CellManager> Ptr;

public:
    Cell::Ptr createCell(IGeometricEntity::Id_t mesh_id, EntityCollection<Face const> const & faces);

    static Ptr create();

private:
    CellManager();
};

#pragma warning(default:4251)
