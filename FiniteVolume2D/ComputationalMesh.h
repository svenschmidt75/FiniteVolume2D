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
#include "FiniteVolume2DLib/IMeshConnectivity.h"

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
    explicit ComputationalMesh(IMeshConnectivity const & mesh_connectivity);

    IMeshConnectivity const & getMeshConnectivity() const;

    /* Computational entities are stored in threads, so
     * they can easily be queried. Also, this is used
     * to construct the various computational grids for
     * the multigrid method.
     */
    Thread<ComputationalNode> &     getNodeThread(IGeometricEntity::Entity_t entity_type);
    Thread<ComputationalFace> &     getFaceThread(IGeometricEntity::Entity_t entity_type);
    Thread<ComputationalCell> &     getCellThread();

    GeometricalEntityMapper const & getMapper() const;

private:
    ComputationalMesh();
    ComputationalMesh(ComputationalMesh const & in);
    ComputationalMesh & operator=(ComputationalMesh const & in);

    // access only from the ComputationalMeshBuilder
    void addNode(Node::Ptr const & node, ComputationalNode::Ptr const & cnode);
    void addFace(Face::Ptr const & face, ComputationalFace::Ptr const & cface);
    void addCell(Cell::Ptr const & cell, ComputationalCell::Ptr const & ccell);

private:
    /* The computational entities hold a reference to their geometrical
     * counterparts. The mesh connectivity is given in terms of the
     * geometrical entities. This mapper allows the mapping from
     * geometric entities to computational ones.
     */
    GeometricalEntityMapper mapper_;

    /* mesh connectivity is inherited from the geometrical mesh */
    IMeshConnectivity const & mesh_connectivity_;

    Thread<ComputationalNode> interior_node_thread_;
    Thread<ComputationalNode> boundary_node_thread_;

    Thread<ComputationalFace> interior_face_thread_;
    Thread<ComputationalFace> boundary_face_thread_;

    Thread<ComputationalCell> cell_thread_;
};
