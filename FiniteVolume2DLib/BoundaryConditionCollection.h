/*
 * Name  : BoundaryConditionCollection
 * Path  : IMeshReaderState
 * Use   : Stores boundary conditions.
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "DeclSpec.h"
 #include "IGeometricEntity.h"

#include <tuple>
#include <map>

#include <boost/optional.hpp>

 
 #pragma warning(disable:4251)
 

class DECL_SYMBOLS BoundaryConditionCollection {
public:
    enum Type {DIRICHLET, NEUMANN, UNKNOWN};
    typedef std::tuple<BoundaryConditionCollection::Type, double> Pair;

public:
    bool                  add(IGeometricEntity::Id_t face_id, Type bc_type, double bc_value);
    boost::optional<Pair> find(IGeometricEntity::Id_t face_id) const;

private:
    typedef std::map<IGeometricEntity::Id_t, Pair> BoundaryCondition_t;

private:
    BoundaryCondition_t data_;
};

#pragma warning(default:4251)
