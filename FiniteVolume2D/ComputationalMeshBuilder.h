/*
 * Name  : ComputationalMeshBuilder
 * Path  : 
 * Use   : Builds a computational mesh from a geometrical mesh.
 *         Nodes -> ComputationalNodes
 *         Faces -> ComputationalFaces
 *         Cells -> ComputationalCells
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "FiniteVolume2DLib/Mesh.h"
#include "ComputationalMesh.h"


class ComputationalMeshBuilder {
public:
    explicit ComputationalMeshBuilder(Mesh::Ptr const & mesh);

    ComputationalMesh::Ptr build() const;

private:
    // geometric mesh to convert to a computational one
    Mesh::Ptr geometrical_mesh_;
};


// replicate cells, nodes, faces
// set boundary condition on faces
// setters for computational molecules
