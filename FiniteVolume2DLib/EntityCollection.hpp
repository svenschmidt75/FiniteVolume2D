/*
 * Name  : EntityCollection
 * Path  : 
 * Use   : Holds entities
 * Author: Sven Schmidt
 * Date  : 03/10/2012
 */
#pragma once

#include <map>

#include "IGeometricEntity.h"


template<typename Entity>
class EntityCollection {

    // template class friend declaration
    template<typename Entity>
    friend class EntityManager;

private:
    typedef typename std::map<IGeometricEntity::Id_t, typename Entity::Ptr> EntityCollection_t;
    typedef typename std::map<IGeometricEntity::Id_t, typename Entity::Ptr>::iterator iterator;

public:
    void insert(typename Entity::Ptr const & entity) {
        data_[entity->id()] = entity;
    }

    // non-const iterator
    iterator begin() {
        return data.begin();
    }

    iterator end() {
        return data.end();
    }

private:
    EntityCollection_t data_;
};
