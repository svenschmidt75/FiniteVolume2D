/*
 * Name  : Thread
 * Path  : IThread
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
    void insert(typename Entity::Ptr const & entity) {
        data_.push_back(entity);
    }

private:
    EntityCollection_t data_;
};
