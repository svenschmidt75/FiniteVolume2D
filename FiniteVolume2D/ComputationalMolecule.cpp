#include "ComputationalMolecule.h"

ComputationalMolecule::ComputationalMolecule()
    :
    ComputationalMoleculeImpl("undef") {}

ComputationalMolecule::ComputationalMolecule(std::string const & var_name)
    :
    ComputationalMoleculeImpl(var_name) {}

void
ComputationalMolecule::add(ComputationalVariable const & cvar, double weight) {
    data_[cvar.id()] = weight;
}

boost::optional<double>
ComputationalMolecule::getWeight(ComputationalVariable const & cvar) const {
    auto it = data_.find(cvar.id());
    if (it == data_.end())
        return boost::optional<double>();
    return it->second;
}

ComputationalMolecule::size_type
ComputationalMolecule::size() const {
    return data_.size();
}
