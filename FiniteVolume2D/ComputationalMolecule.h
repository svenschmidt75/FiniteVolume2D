/*
 * Name  : ComputationalMolecule
 * Path  : 
 * Use   : Represents a computational molecule.
 *         Encodes the dependency on computational nodes.
 *         For example, in order to compute the flux
 *         across cell faces, we need to compute the flux
 *         variable at face vertices via some sort of inter-
 *         polation, as the variables are only "known" at
 *         cell nodes. A computational molecule contains
 *         pairs of cell node ids and weights, which encode
 *         such a dependency.
 * Author: Sven Schmidt
 * Date  : 01/14/2012
 */
#pragma once

#include <map>

#include <boost/optional.hpp>

#include "IGeometricEntity.h"


namespace NS_ComputationalNode {

    class ComputationalMolecule {

        friend class ComputationalMoleculeConstIterator;

    public:
        typedef std::map<IGeometricEntity::Id_t, double>::value_type Type;

    public:
        // Insert a dependency on cell node id with weight
        void insert(IGeometricEntity::Id_t id, double weight);

        // Return the weight for an elements
        boost::optional<double> getWeight(IGeometricEntity::Id_t id) const;

    private:
        // Pair: Computational node id, weight
        std::map<IGeometricEntity::Id_t, double> data_;
    };

}
