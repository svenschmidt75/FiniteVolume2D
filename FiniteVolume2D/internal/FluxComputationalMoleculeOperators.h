/*
 * Name  : FluxComputationalMoleculeOperators
 * Path  : 
 * Use   : Some operators on FluxComputationalMolecules.
 *         NOT to be included from outside FluxComputationalMolecule.h!!!
 * Author: Sven Schmidt
 * Date  : 06/17/2012
 */
#pragma once

#include "../DeclSpec.h"

#include "../FluxComputationalMolecule.h"
#include "../ComputationalMoleculeImpl.h"


DECL_SYMBOLS_2D FluxComputationalMolecule operator-(FluxComputationalMolecule const & in);
DECL_SYMBOLS_2D ComputationalMoleculeImpl & operator+=(ComputationalMoleculeImpl & lhs, ComputationalMoleculeImpl const & rhs);
