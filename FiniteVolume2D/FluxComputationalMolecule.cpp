#include "FluxComputationalMolecule.h"

#include "ComputationalCell.h"
#include "ComputationalMolecule.h"
#include "ComputationalVariableHolder.h"


FluxComputationalMolecule::FluxComputationalMolecule()
    :
    ComputationalMoleculeImpl("undef", nullptr) {}

FluxComputationalMolecule::FluxComputationalMolecule(std::string const & name, ComputationalVariableHolder::Ptr const & cvar_holder)
    :
    ComputationalMoleculeImpl(name, cvar_holder) {}

void
FluxComputationalMolecule::setCell(ComputationalCell::Ptr const & ccell) {
    ccell_ = ccell;
}

ComputationalCell::Ptr const &
FluxComputationalMolecule::getCell() const {
    return ccell_;
}

bool
FluxComputationalMolecule::addMolecule(ComputationalMolecule & in) const {
    return ComputationalMoleculeImpl::addMolecule(in);
}
