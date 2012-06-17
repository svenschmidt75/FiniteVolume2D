#include "ComputationalMoleculeOperators.h"

#include "../ComputationalMoleculeImpl.h"


ComputationalMoleculeImpl
operator-(ComputationalMoleculeImpl const & in) {
    ComputationalMoleculeImpl tmp = in;
    return tmp;
}
