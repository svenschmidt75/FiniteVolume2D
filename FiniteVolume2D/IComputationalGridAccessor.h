/*
 * Name  : IComputationalGridAccessor
 * Path  : 
 * Use   : Allows the user-supplied methods for flux evaluation
 *         to access the computational grid, for example to
 *         query the mesh connectivity etc.
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "DeclSpec.h"

#include "ComputationalCell.h"
#include "ComputationalFace.h"


class IMeshConnectivity;
class GeometricalEntityMapper;


class DECL_SYMBOLS_2D IComputationalGridAccessor {
public:
    IComputationalGridAccessor(IMeshConnectivity const & mesh_connectivity, GeometricalEntityMapper const & mapper);

    ComputationalCell::Ptr getOtherCell(ComputationalFace::Ptr const & face, ComputationalCell::Ptr const & cell) const;

private:
    // no assignment, no copy-construction
    IComputationalGridAccessor(IComputationalGridAccessor const & in);
    IComputationalGridAccessor & operator==(IComputationalGridAccessor const & in);

private:
    IMeshConnectivity const &       mesh_connectivity_;
    GeometricalEntityMapper const & mapper_;
};
