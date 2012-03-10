/*
 * Name  : EntityMgr
 * Path  : 
 * Use   : Entity manager base class
 * Author: Sven Schmidt
 * Date  : 03/10/2012
 */
#pragma once

#include "EntityCollection.hpp"


template<typename Entity>
class EntityManager {
public:
    typedef typename EntityCollection<Entity>::iterator iterator;

public:
    iterator begin();
    iterator end();

private:
    IGeometricEntity::Id_t index() {
        static IGeometricEntity::Id_t index = 0;
        return index++;
    }


protected:
    EntityCollection<Entity> collection_;
};
