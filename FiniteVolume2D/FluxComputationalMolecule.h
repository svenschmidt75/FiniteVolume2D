/*
 * Name  : FluxComputationalMolecule
 * Path  : ComputationalMoleculeImpl
 * Use   : Computational molecules for comp. faces
 * Author: Sven Schmidt
 * Date  : 04/21/2012
 */
#pragma once

#include "ComputationalMoleculeImpl.h"


class FluxComputationalMolecule : public ComputationalMoleculeImpl {
public:
    FluxComputationalMolecule();
    explicit FluxComputationalMolecule(std::string const & name);

    using ComputationalMoleculeImpl::name;
};
