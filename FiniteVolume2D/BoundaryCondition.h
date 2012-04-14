/*
 * Name  : BoundaryCondition
 * Path  : 
 * Use   : Boundary condition for boundary faces
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "FiniteVolume2DLib/BoundaryConditionCollection.h"

#include <memory>


class BoundaryCondition {
public:
    typedef std::shared_ptr<BoundaryCondition> Ptr;

public:
    explicit BoundaryCondition(BoundaryConditionCollection::Pair const & bc);

    double                            getValue() const;
    BoundaryConditionCollection::Type type() const;

private:
    double                            value_;
    BoundaryConditionCollection::Type type_;
};
