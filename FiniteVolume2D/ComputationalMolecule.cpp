#include "ComputationalMolecule.h"


ComputationalMolecule::ComputationalMolecule()
    :
    ComputationalMoleculeImpl("undef") {}

ComputationalMolecule::ComputationalMolecule(std::string const & var_name)
    :
    ComputationalMoleculeImpl(var_name) {}
