/*
 * Name  : ComputationalMeshMock
 * Path  : IComputationalMesh
 * Use   : Represents a ComputationalMesh that contains
 *         some arbitrary state.
 *         
 *         Note: Using this object as a "facade", we can have
 *         this object behave as if it were a real computational
 *         mesh with out having to set up the internal (possibly
 *         expensive and complicated) data structures. For all
 *         intends and purposes, this object "is" a computational
 *         mesh.
 *         
 * Author: Sven Schmidt
 * Date  : 06/23/2012
 */
#pragma once

#include "FiniteVolume2D/IComputationalMesh.h"

class ComputationalMeshMock : public IComputationalMesh {
public:
    ComputationalMeshMock() {

    }

};
