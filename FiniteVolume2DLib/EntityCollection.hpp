/*
 * Name  : EntityCollection
 * Path  : 
 * Use   : Holds entities
 * Author: Sven Schmidt
 * Date  : 03/10/2012
 */
#pragma once

#include <vector>

#include "IGeometricEntity.h"


template<typename Entity>
class EntityCollection {

    // template class friend declaration
    template<typename Entity>
    friend class EntityManager;

private:
    typedef typename std::vector<typename Entity::Ptr> EntityCollection_t;
    typedef typename std::vector<typename Entity::Ptr>::iterator iterator;

public:
    typedef typename EntityCollection_t::size_type size_type;

public:
    void insert(typename Entity::Ptr const & entity) {
        data_.push_back(entity);
    }

    // non-const iterator
    iterator begin() {
        return data_.begin();
    }

    iterator end() {
        return data_.end();
    }

    size_type size() const {
        return data_.size();
    }

    typename Entity::Ptr & getEntity(size_type index) {
        if (index >= size())
            throw std::out_of_range("EntityCollection::getEntity: Out of range!");

        return data_[index];
    }

    typename Entity::Ptr const & getEntity(size_type index) const {
        if (index >= size())
            throw std::out_of_range("EntityCollection::getEntity: Out of range!");

        return data_[index];
    }

private:
    EntityCollection_t data_;
};
