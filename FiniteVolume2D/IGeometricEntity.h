/*
 * Name  : IGeometricEntity
 * Path  : 
 * Use   : Base class for vertices, faces, cells
 * Author: Sven Schmidt
 * Date  : 01/29/2012
 */
#pragma once

class IGeometricEntity {
public:
    typedef size_t Id_t;

public:
    ~IGeometricEntity() {}

    // Unique id of this entity
    virtual Id_t id() const = 0;
};
