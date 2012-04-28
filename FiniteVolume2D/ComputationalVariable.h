/*
 * Name  : ComputationalVariable
 * Path  : 
 * Use   : Computational variables are used to uniquely indicate a
 *         variable that is solved for, like Temperature or Pressure
 *         in the computational molecules of the cells/faces.
 * Author: Sven Schmidt
 * Date  : 04/28/2012
 */
#pragma once

#include "DeclSpec.h"

#include <memory>
#include <string>


class ComputationalCell;


class ComputationalVariable {

    friend class ComputationalVariableManager;

public:
    typedef std::shared_ptr<ComputationalVariable> Ptr;
    typedef long long int Id_t;

public:
    explicit ComputationalVariable(std::shared_ptr<ComputationalCell> const & cell, std::string const & name, Id_t unique_id);

    bool operator==(ComputationalVariable const & in) const;

    std::string const & getName() const;

    /* By using "std::shared_ptr<ComputationalCell>' instead of
     * ComputationalCell::Ptr, we are not required to include
     * ComputationalCell.h; forward-declaration is sufficient.
     */
    static Ptr create(std::shared_ptr<ComputationalCell> const & cell, std::string const & name, Id_t unique_id);

private:
    // restrict creation to ComputationalCellManager only
    ComputationalVariable();

private:
    // the computational cell this variable is tied to
    std::shared_ptr<ComputationalCell> cell_;
    
    // the name of this variable, that will be solved for
    std::string name_;
    
    /* each comp. variable has a unique id.
     * This is used in the ComputationalMolecules
     * and as the index of this variable in the sparse
     * linear system later on.
     */
    Id_t unique_id_;
};
