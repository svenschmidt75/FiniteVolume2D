/*
 * Name  : BoundaryCondition
 * Path  : IMeshReaderState
 * Use   : Stores boundary conditions.
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "IGeometricEntity.h"

#include <tuple>
#include <map>

#include <boost/optional.hpp>


class BoundaryCondition {
public:
    enum Type {DIRICHLET, NEUMANN, UNKNOWN};
    typedef std::tuple<BoundaryCondition::Type, double> Pair;

public:
    bool                  add(IGeometricEntity::Id_t face_id, Type bc_type, double bc_value);
    boost::optional<Pair> find(IGeometricEntity::Id_t face_id) const;

private:
    typedef std::map<IGeometricEntity::Id_t, Pair> BoundaryCondition_t;

private:
    BoundaryCondition_t data_;
};
