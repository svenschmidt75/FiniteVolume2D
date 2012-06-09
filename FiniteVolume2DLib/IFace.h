/*
 * Name  : IFace
 * Path  : IGeometricEntity
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "IGeometricEntity.h"

#include "Node.h"
#include "EntityCollection.hpp"
#include "Vertex.h"
#include "Vector.h"


class IFace : public IGeometricEntity {
public:
    virtual ~IFace() {}

    virtual IGeometricEntity::Entity_t           getEntityType() const = 0;
    virtual EntityCollection<Node const> const & getNodes() const = 0;
    virtual double                               area() const = 0;
    virtual Vector                               normal() const = 0;
    virtual Vertex                               centroid() const = 0;
};
