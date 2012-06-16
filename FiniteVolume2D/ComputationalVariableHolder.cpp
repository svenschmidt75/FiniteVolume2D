#include "ComputationalVariableHolder.h"

#include <exception>


void
ComputationalVariableHolder::add(ComputationalVariable::Ptr const & cvar) {
    holder_[cvar->id()] = cvar;
}

ComputationalVariable::Ptr const &
ComputationalVariableHolder::get(ComputationalVariable::Id_t const & cvar_id) const {
    auto it = holder_.find(cvar_id);
    if (it == holder_.end())
        throw std::logic_error("ComputationalVariableHolder::get: Computational cell not found");

    return it->second;
}
