#include "ComputationalMoleculeImpl.h"

#include "ComputationalCell.h"
#include "ComputationalVariableManager.h"


#include <iostream>
#include <algorithm>


ComputationalMoleculeImpl::ComputationalMoleculeImpl(std::string const & name)
    :
    name_(name) {}

void
ComputationalMoleculeImpl::printMolecule(ComputationalVariableManager const & cvar_mgr) const {
    // print computational molecule

    std::cout << std::endl << "Molecule name: " << name() << std::endl;

    std::for_each(data_.begin(), data_.end(), [this, &cvar_mgr](ComputationalMolecule_t::value_type const & item) {
        // item.first  : ComputationalVariable::Id_t
        // item.second : weight
        // find this contribution in "in"
        ComputationalVariable::Id_t const & cvar_id = item.first;
        double const & weight = item.second;

        ComputationalVariable::Ptr cvar = cvar_mgr.getComputationalVariable(cvar_id);
        ComputationalCell::Ptr const & ccell = cvar->getCell();

        std::cout << "(Cell id, var name, weight) = (" << ccell->geometricEntity()->meshId() << ", " << cvar->getName() << ", " << weight << ")" << std::endl;
    });

    std::cout << "Source term: " << source_term_.value() << std::endl;
}

void
ComputationalMoleculeImpl::negate() {
    std::for_each(data_.begin(), data_.end(), [](ComputationalMolecule_t::value_type & item) {
        // item.first  : ComputationalVariable::Id_t
        // item.second : weight
        // find this contribution in "in"
        double & weight = item.second;
        weight *= -1.0;
    });
}

void
ComputationalMoleculeImpl::print(ComputationalVariableManager const & cvar_mgr) const {
    // print computational molecule
    printMolecule(cvar_mgr);
}

void
ComputationalMoleculeImpl::print(ComputationalNode const & cnode, ComputationalVariableManager const & cvar_mgr) const {
    // print computational molecule

    std::cout << std::endl << "(Node, molecule) = (" << cnode.geometricEntity()->meshId() << ", " << name() << ")" << std::endl;

    printMolecule(cvar_mgr);
}

void
ComputationalMoleculeImpl::print(ComputationalFace const & cface, ComputationalVariableManager const & cvar_mgr) const {
    // print computational molecule

    std::cout << std::endl << "(Face, molecule) = (" << cface.geometricEntity()->meshId() << ", " << name() << ")" << std::endl;

    printMolecule(cvar_mgr);
}

void
ComputationalMoleculeImpl::print(ComputationalCell const & ccell, ComputationalVariableManager const & cvar_mgr) const {
    // print computational molecule

    std::cout << std::endl << "(Cell, molecule) = (" << ccell.geometricEntity()->meshId() << ", " << name() << ")" << std::endl;

    printMolecule(cvar_mgr);
}

std::string const &
ComputationalMoleculeImpl::name() const {
    return name_;
}

void
ComputationalMoleculeImpl::add(ComputationalVariable const & cvar, double weight) {
    data_[cvar.id()] += weight;
}

boost::optional<double>
ComputationalMoleculeImpl::getWeight(ComputationalVariable const & cvar) const {
    auto it = data_.find(cvar.id());
    if (it == data_.end())
        return boost::optional<double>();
    return it->second;
}

ComputationalMoleculeImpl::size_type
ComputationalMoleculeImpl::size() const {
    return data_.size();
}

bool
ComputationalMoleculeImpl::empty() const {
    return data_.empty();
}

SourceTerm &
ComputationalMoleculeImpl::getSourceTerm() {
    return source_term_;
}

SourceTerm const &
ComputationalMoleculeImpl::getSourceTerm() const {
    return source_term_;
}

ComputationalMoleculeImpl::Iterator_t
ComputationalMoleculeImpl::begin() const {
    return data_.begin();
}

ComputationalMoleculeImpl::Iterator_t
ComputationalMoleculeImpl::end() const {
    return data_.end();
}

bool
ComputationalMoleculeImpl::addMolecule(ComputationalMoleculeImpl & in) const {
    std::for_each(data_.begin(), data_.end(), [&in](ComputationalMolecule_t::value_type const & item) {
        // item.first  : ComputationalVariable::Id_t
        // item.second : weight
        // find this contribution in "in"
        ComputationalVariable::Id_t const & cvar_id = item.first;
        double const & weight = item.second;

        auto it = in.data_.find(cvar_id);
        if (it == in.data_.end())
            in.data_[cvar_id] = weight;

        else
            in.data_[cvar_id] += weight;
    });

    // account for source term
    in.getSourceTerm() += getSourceTerm().value();

    return true;
}
