/*
 * Name  : FluxComputationalMolecule
 * Path  : ComputationalMoleculeImpl
 * Use   : Computational molecules for comp. faces
 * Author: Sven Schmidt
 * Date  : 04/21/2012
 */
#pragma once

#include "DeclSpec.h"

#include "ComputationalMoleculeImpl.h"

#include <memory>


class ComputationalCell;
class ComputationalMolecule;


#pragma warning(disable:4251)


class DECL_SYMBOLS_2D FluxComputationalMolecule : public ComputationalMoleculeImpl {

    friend DECL_SYMBOLS_2D FluxComputationalMolecule operator-(FluxComputationalMolecule const & in);

public:
    FluxComputationalMolecule();
    explicit FluxComputationalMolecule(std::string const & name);


    void                                       setCell(std::shared_ptr<ComputationalCell> const & ccell);
    std::shared_ptr<ComputationalCell> const & getCell() const;

    bool                                       addMolecule(ComputationalMolecule & in) const;

private:
    void negate();

private:
    /* Flux through non-boundary faces involves two cells.
     * The flux has the negative sign for the other cell
     * involved. In order to avoid a 2nd computation, we
     * store the cell the face flux was evaluated with
     * here.
     */
    std::shared_ptr<ComputationalCell> ccell_;
};

#pragma warning(default:4251)

// common operator overloads for FluxComputationalMolecule
#include "internal/FluxComputationalMoleculeOperators.h"
