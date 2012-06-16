/*
 * Name  : ComputationalVariableHolder
 * Path  : 
 * Use   : Keeps all comp. variables
 * Author: Sven Schmidt
 * Date  : 04/16/2012
 */
#pragma once

#include "ComputationalVariable.h"

#include <unordered_map>
#include <memory>


class ComputationalVariableHolder {
public:
    typedef std::shared_ptr<ComputationalVariableHolder> Ptr;
    typedef std::unordered_map<ComputationalVariable::Id_t, ComputationalVariable::Ptr> Holder_t;

public:
    void                               add(ComputationalVariable::Ptr const & cvar);
    ComputationalVariable::Ptr const & get(ComputationalVariable::Id_t const & cvar_id) const;

private:
    Holder_t holder_;
};
