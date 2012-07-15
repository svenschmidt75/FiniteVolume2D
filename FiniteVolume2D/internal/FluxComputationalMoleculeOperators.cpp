#include "FluxComputationalMoleculeOperators.h"


FluxComputationalMolecule
operator-(FluxComputationalMolecule const & in) {
    FluxComputationalMolecule tmp = in;
    tmp.negate();
    return tmp;
}

ComputationalMoleculeImpl &
operator+=(ComputationalMoleculeImpl & lhs, ComputationalMoleculeImpl const & rhs) {
    rhs.addMolecule(lhs);
    return lhs;
}

