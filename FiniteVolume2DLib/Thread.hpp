/*
 * Name  : Thread
 * Path  : 
 * Use   : Thread class. Contains elements of a certain type
 *         like vertices, faces and cells.
 * Author: Sven Schmidt
 * Date  : 03/10/2012
 */
#pragma once

#include <deque>


template<typename Entity>
class Thread {
private:
    typedef typename std::deque<typename Entity::Ptr> EntityCollection_t;

public:
    typedef typename Entity                             type;
    typedef typename EntityCollection_t::size_type      size_type;
    typedef typename EntityCollection_t::const_iterator iterator;

public:
    size_type insert(typename Entity::Ptr const & entity) {
        size_type index = size();
        data_.push_back(entity);
        return index;
    }

    typename EntityCollection_t::size_type size() const {
        return data_.size();
    }

    typename Entity::Ptr const getEntityAt(size_type index) const {
        return data_[index];
    }

    // add begin() and end() so we can use STL algorithms
    iterator begin() const {
        return data_.begin();
    }

    iterator end() const {
        return data_.end();
    }

private:
    EntityCollection_t data_;
};
