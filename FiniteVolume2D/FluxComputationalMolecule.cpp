#include "FluxComputationalMolecule.h"

#include "ComputationalCell.h"
#include "ComputationalMolecule.h"


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
FluxComputationalMolecule::getCell() const {
    return ccell_;
}

bool
FluxComputationalMolecule::addMolecule(ComputationalMolecule & in) const {
    return ComputationalMoleculeImpl::addMolecule(in);
}

void
FluxComputationalMolecule::negate() {
    ComputationalMoleculeImpl::negate();
}
