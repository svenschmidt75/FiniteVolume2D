#include "BoundaryCondition.h"

BoundaryCondition::BoundaryCondition(BoundaryConditionCollection::Pair const & bc) : type_(std::get<0>(bc)), value_(std::get<1>(bc)) {}

double
BoundaryCondition::getValue() const {
    return value_;
}

BoundaryConditionCollection::Type
BoundaryCondition::type() const {
    return type_;
}
