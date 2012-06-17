#include "FluxComputationalMoleculeOperators.h"

FluxComputationalMolecule
operator-(FluxComputationalMolecule const & in) {
    FluxComputationalMolecule tmp = in;
    tmp.negate();
    return tmp;
}
