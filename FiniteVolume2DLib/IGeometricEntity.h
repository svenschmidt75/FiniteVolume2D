/*
 * Name  : IGeometricEntity
 * Path  : 
 * Use   : Base class for vertices, faces, cells
 * Author: Sven Schmidt
 * Date  : 01/29/2012
 */
#pragma once

#include "DeclSpec.h"

#include <boost/cstdint.hpp>
#include <boost/limits.hpp>


class DECL_SYMBOLS_2DLIB IGeometricEntity {
public:
    typedef boost::uint64_t Id_t;
 
    // entity type
    enum Entity_t {BOUNDARY, INTERIOR, UNKNOWN};

public:
    ~IGeometricEntity() {}

    // Unique id of this entity
    virtual Id_t id() const = 0;
    virtual Id_t meshId() const = 0;

    static Id_t undef() { return std::numeric_limits<Id_t>::max(); }
};
