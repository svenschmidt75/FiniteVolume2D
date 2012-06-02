#include "FluxComputationalMolecule.h"

#include "ComputationalCell.h"


FluxComputationalMolecule::FluxComputationalMolecule()
    :
    ComputationalMoleculeImpl("undef") {}

FluxComputationalMolecule::FluxComputationalMolecule(std::string const & name)
    :
    ComputationalMoleculeImpl(name) {}

void
FluxComputationalMolecule::setCell(ComputationalCell::Ptr const & ccell) {
    ccell_ = ccell;
}

ComputationalCell::Ptr const &
FluxComputationalMolecule::getCell(void) const {
    return ccell_;
}
