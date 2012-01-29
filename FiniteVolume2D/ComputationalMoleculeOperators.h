/*
 * Name  : ComputationalMoleculeOperators
 * Path  : 
 * Use   : Operators for adding computational
 *         nodes, etc.
 * Author: Sven Schmidt
 * Date  : 01/29/2012
 */
#pragma once

#include "ComputationalMolecule.h"


namespace NS_ComputationalNode {

    class ComputationalMoleculeOperators {
    public:
        static ComputationalMolecule operator+(ComputationalMolecule const & lhs, ComputationalMolecule const & rhs);

    };

}

