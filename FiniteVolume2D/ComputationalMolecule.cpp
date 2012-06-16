#include "ComputationalMolecule.h"

#include "FluxComputationalMolecule.h"
#include "ComputationalVariableHolder.h"


ComputationalMolecule::ComputationalMolecule()
    :
    ComputationalMoleculeImpl("undef", nullptr) {}

ComputationalMolecule::ComputationalMolecule(std::string const & var_name, ComputationalVariableHolder::Ptr const & cvar_holder)
    :
    ComputationalMoleculeImpl(var_name, cvar_holder) {}

bool
ComputationalMolecule::addMolecule(FluxComputationalMolecule const & in) {
    return in.addMolecule(*this);
}
