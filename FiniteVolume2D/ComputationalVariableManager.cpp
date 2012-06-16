#include "ComputationalVariableManager.h"

#include "ComputationalVariable.h"
#include "internal/ComputationalVariableManagerIterator.h"

#include "FiniteVolume2DLib/Util.h"


#include <memory>

#include <boost/format.hpp>


ComputationalVariableManager::ComputationalVariableManager()
    :
    index_(0),
    is_finialized_(false),
    cvar_holder_(std::make_shared<ComputationalVariableHolder>()) {}


ComputationalVariable::Ptr
ComputationalVariableManager::create(ComputationalCell::Ptr const & cell, std::string const & name) {
    // check whether this variable has been registered
    auto it = variables_.find(name);
    if (it == variables_.end()) {
        boost::format format = boost::format("ComputationalVariableManager::create: Computational variable %1% must be registered first!\n") % name;
        Util::error(format.str());
        return false;
    }

    // no more comp. variable additions possible
    is_finialized_ = true;

    if (cell->getComputationalVariable(name)) {
        boost::format format = boost::format("ComputationalVariableManager::create: Variable %1% already defined!\n") % name;
        Util::error(format.str());
        return false;
    }

    // create the unique id for the comp. variable
    ComputationalVariable::Id_t cvar_id = cell->id() + getBaseIndex(name);

    ComputationalVariable::Ptr cvar = ComputationalVariable::create(cell, name, cvar_id);
    cvar_holder_->add(cvar);
    cell->addComputationalVariable(cvar, cvar_holder_);
    return cvar;
}

bool
ComputationalVariableManager::registerVariable(std::string const & name, FluxEvaluator_t const & flux_eval) {
    if (is_finialized_) {
        boost::format format = boost::format("ComputationalVariableManager::registerVariable: Unable to add computational variable %1%\n \
            as create() has already been called!\n") % name;
        Util::error(format.str());
        return false;
    }

    // check whether there is already a comp. variable
    auto it = variables_.find(name);
    if (it != variables_.end()) {
        boost::format format = boost::format("ComputationalVariableManager::registerVariable: Computational variable %1% already defined!\n") % name;
        Util::error(format.str());
        return false;
    }

    ComputationalVariableItem & cvar_item = variables_[name];
    cvar_item.index = index_++;
    cvar_item.flux_eval = flux_eval;

    return true;
}

ComputationalVariableHolder::Ptr const &
ComputationalVariableManager::getComputationalVariableHolder() const {
    return cvar_holder_;
}

short
ComputationalVariableManager::getBaseIndex(std::string const & cvar_name) const {
    auto it = variables_.find(cvar_name);
    if (it == variables_.end())
        return -1;
    return it->second.index;
}

ComputationalVariableManager::size_type
ComputationalVariableManager::size() const {
    return variables_.size();
}

ComputationalVariableManagerIterator
ComputationalVariableManager::begin() const {
    return ComputationalVariableManagerIterator(variables_);
}

ComputationalVariableManagerIterator
ComputationalVariableManager::end() const {
    return ComputationalVariableManagerIterator();
}
