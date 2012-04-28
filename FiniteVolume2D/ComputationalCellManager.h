/*
 * Name  : ComputationalCellManager
 * Path  : 
 * Use   : Creates computational variables.
 * Author: Sven Schmidt
 * Date  : 04/28/2012
 */
#pragma once

#include "DeclSpec.h"

#include <memory>
#include <unordered_map>
#include <string>


class ComputationalVariable;


class ComputationalCellManager {
public:
    std::shared_ptr<ComputationalVariable> create(std::shared_ptr<ComputationalCell> const & cell, std::string const & name);

    void register(std::string const & name);

    short getBaseIndex(ComputationalVariable const & var) const;

private:
    typedef std::unordered_map<std::string, short> VariableBaseIndexMap_t;

private:
    /* Each computational variable has a unique index.
     * This index is used in the computational molecules
     * and also in the sparse linear system.
     * Assume variables T(emperature) and P(ressure) have
     * been registered. Then, the base index is
     * T: 0
     * P: 1
     * This is used to order the linear system such that first,
     * all Ts appear, then all Ps.
     */
    VariableBaseIndexMap_t var_index_map_;

    /* After the first call of create, we do not allow further
     * additions (register) of computational variables, as this
     * would mess up all the indexing.
     */
    bool is_finialized_;
};
