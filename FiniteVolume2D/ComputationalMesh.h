/*
 * Name  : ComputationalMesh
 * Path  : 
 * Use   : Computational mesh
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "FiniteVolume2DLib/Mesh.h"
#include "FiniteVolume2DLib/Thread.hpp"

#include "GeometricalEntityMapper.h"
#include "ComputationalCell.h"
#include "ComputationalFace.h"
#include "ComputationalNode.h"

#include <memory>


class ComputationalMesh {

    friend class ComputationalMeshBuilder;

public:
    typedef std::shared_ptr<ComputationalMesh> Ptr;

public:
    ComputationalMesh();

    /* Computational entities are stored in threads, so
     * they can easily be queried. Also, this is used
     * to construct the various computational grids for
     * the multigrid method.
     */
    Thread<ComputationalNode> & getBoundaryVertexThread();
    Thread<ComputationalNode> & getInteriorVertexThread();

    Thread<ComputationalFace> & getBoundaryFaceThread();
    Thread<ComputationalFace> & getInteriorFaceThread();

    Thread<ComputationalCell> & getCellThread();

private:
    // access only from the ComputationalMeshBuilder
    void addNode(ComputationalNode::Ptr const & node);
    void addFace(ComputationalFace::Ptr const & face);
    void addCell(ComputationalCell::Ptr const & cell);

private:
    /* The computational entities hold a reference to their geometrical
     * counterparts. The mesh connectivity is given in terms of the
     * geometrical entities. This mapper allows the mapping from
     * geometric entities to computational ones.
     */
    GeometricalEntityMapper mapper_;
};
