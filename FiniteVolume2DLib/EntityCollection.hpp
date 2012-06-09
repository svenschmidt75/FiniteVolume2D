/*
 * Name  : EntityCollection
 * Path  : 
 * Use   : Holds entities
 * Author: Sven Schmidt
 * Date  : 03/10/2012
 */
#pragma once

#include "IGeometricEntity.h"

#include <vector>

#include <boost/optional.hpp>


template<typename Entity>
class EntityCollection {

    // template class friend declaration
    template<typename Entity>
    friend class EntityManager;

private:
    typedef typename std::vector<typename Entity::CPtr> EntityCollection_t;
    typedef typename Entity EntityType;
    typedef typename std::vector<typename Entity::CPtr>::iterator iterator;
    typedef typename std::vector<typename Entity::CPtr>::const_iterator const_iterator;

public:
    typedef typename EntityCollection_t::size_type size_type;

public:
    void insert(typename Entity::CPtr const & entity) {
        data_.push_back(entity);
    }

    void insertUnique(typename Entity::CPtr const & entity) {
        if (!find(entity->meshId()))
            data_.push_back(entity);
    }

    // const iterator
    const_iterator begin() const {
        return data_.begin();
    }

    const_iterator end() const {
        return data_.end();
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

    typename Entity::Ptr & operator[](size_type index) {
        if (index >= size())
            throw std::out_of_range("EntityCollection::getEntity: Out of range!");

        return data_[index];
    }

    typename Entity::CPtr getEntity(size_type index) const {
        if (index >= size())
            throw std::out_of_range("EntityCollection::getEntity: Out of range!");

        return data_[index];
    }

    typename Entity::CPtr operator[](size_type index) const {
        if (index >= size())
            throw std::out_of_range("EntityCollection::getEntity: Out of range!");

        return data_[index];
    }

    typename boost::optional<typename Entity::CPtr> find(IGeometricEntity::Id_t id) const {
        const_iterator it = std::find_if(data_.begin(), data_.end(), [id](typename Entity::CPtr const & entity){
            return entity->meshId() == id;
        });

        if (it != data_.end())
            return *it;

        return boost::optional<Entity::CPtr>();
    }

private:
    EntityCollection_t data_;
};
