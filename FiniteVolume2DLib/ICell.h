/*
 * Name  : ICell
 * Path  : IGeometricEntity
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "IGeometricEntity.h"

#include "EntityCollection.hpp"
#include "Node.h"
#include "Face.h"
#include "Vector.h"
#include "Vertex.h"


class ICell : public IGeometricEntity {
public:
    virtual ~ICell() {}

    virtual EntityCollection<Node const> const & getNodes() const = 0;
    virtual EntityCollection<Face const> const & getFaces() const = 0;
    virtual double                               volume() const = 0;
    virtual Vertex                               centroid() const = 0;

    virtual Vector                               faceNormal(Face::CPtr const & face) const = 0;
};
