/*
 * Name  : IComputationalVertex
 * Path  : Vertex
 * Use   : Represents a vertex with computational molecule.
 *         On the one hand, we have the mesh, which is
 *         a geometry object. On the other hand, we have
 *         the computational grid, which contains all the
 *         computational nodes etc.
 * Author: Sven Schmidt
 * Date  : 01/29/2012
 */
#pragma once

#include "Vertex.h"
#include "ComputationalMolecule.h"


class IComputationalVertex : public Node {
public:


private:
    // The vertex this object represents.
    Node::Ptr vertex_;

    // Dependency on computational nodes
    ComputationalMolecule cm_;
};
