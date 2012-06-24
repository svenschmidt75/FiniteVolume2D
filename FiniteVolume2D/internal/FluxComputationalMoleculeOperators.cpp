#include "FluxComputationalMoleculeOperators.h"

FluxComputationalMolecule
operator-(FluxComputationalMolecule const & in) {
    FluxComputationalMolecule tmp = in;
    tmp.negate();
    return tmp;
}
// 
// ComputationalMoleculeImpl &
// operator+=(ComputationalMoleculeImpl & lhs, ComputationalMoleculeImpl const & rhs) {
//     lhs.addMolecule(rhs);
//     return lhs;
// }
