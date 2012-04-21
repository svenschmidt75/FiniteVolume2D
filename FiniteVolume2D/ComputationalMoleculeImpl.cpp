#include "ComputationalMoleculeImpl.h"

ComputationalMoleculeImpl::ComputationalMoleculeImpl(std::string const & name) : name_(name) {}

std::string const &
ComputationalMoleculeImpl::name() const {
    return name_;
}
