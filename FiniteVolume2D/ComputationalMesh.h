/*
 * Name  : ComputationalMesh
 * Path  : 
 * Use   : Computational mesh
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "FiniteVolume2DLib/Mesh.h"
#include "GeometricalEntityMapper.h"

#include <memory>


class ComputationalMesh {
public:
    typedef std::shared_ptr<ComputationalMesh> Ptr;

public:
    ComputationalMesh(Mesh const & mesh);


private:
    /* The computational entities hold a reference to their geometrical
     * counterparts. The mesh connectivity is given in terms of the
     * geometrical entities. This mapper allows the mapping from
     * geometric entities to computational ones.
     */
    GeometricalEntityMapper mapper_;
};
