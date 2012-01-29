/*
 * Name  : ComputationalMolecule
 * Path  : 
 * Use   : Represents a computational molecule.
 *         To approximate a derivative at a node, we describe its dependencies
 *         together with a weight as a computational molecule.
 *         nodes with Dirichlet b.c.
 * Author: Sven Schmidt
 * Date  : 01/14/2011
 */
#pragma once

class ComputationalMolecule {
public:

    void insert(NodeId);

};