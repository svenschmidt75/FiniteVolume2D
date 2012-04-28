#include "ComputationalVariableManager.h"

#include "ComputationalVariable.h"

#include "FiniteVolume2DLib/Util.h"

#include <boost/format.hpp>


ComputationalVariableManager::ComputationalVariableManager() : index_(0), is_finialized_(false) {}


ComputationalVariable::Ptr
ComputationalVariableManager::create(std::shared_ptr<ComputationalCell> const & cell, std::string const & name) {
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

short
ComputationalVariableManager::getBaseIndex(std::string const & cvar_name) const {
    auto it = variables_.find(cvar_name);
    if (it == variables_.end())
        return -1;
    return it->second.index;
}

short
ComputationalVariableManager::size() const {
    return variables_.size();
}

std::tuple<std::string, ComputationalVariableManager::FluxEvaluator_t>
ComputationalVariableManager::get(short index) const {
}

