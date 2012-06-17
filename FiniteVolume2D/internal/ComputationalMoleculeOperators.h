/*
 * Name  : ComputationalMoleculeOperators
 * Path  : 
 * Use   : Some operators on ComputationalMolecules.
 *         NOT to be included from outside ComputationalMolecule.h!!!
 * Author: Sven Schmidt
 * Date  : 06/17/2012
 */
#pragma once

#include "../DeclSpec.h"

#include "../ComputationalMoleculeImpl.h"

DECL_SYMBOLS_2D ComputationalMoleculeImpl operator-(ComputationalMoleculeImpl const & in);
