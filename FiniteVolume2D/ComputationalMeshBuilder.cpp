#include "ComputationalMeshBuilder.h"

#include "FiniteVolume2DLib/Thread.hpp"
#include "FiniteVolume2DLib/Node.h"

ComputationalMeshBuilder::ComputationalMeshBuilder(Mesh::Ptr const & geometrical_mesh) : geometrical_mesh_(geometrical_mesh) {}

ComputationalMesh::Ptr
ComputationalMeshBuilder::build() const {
    ComputationalMesh::Ptr cmesh(new ComputationalMesh);

    // build computational nodes
    Thread<Node> const & interior_node_thread = geometrical_mesh_->getInteriorNodeThread();
    for (Thread<Node>::size_type i = 0; i < interior_node_thread.size(); ++i)
        cmesh->addNode(ComputationalNode::Ptr(new ComputationalNode(interior_node_thread.getEntityAt(i))));

    Thread<Node> const & boundary_node_thread = geometrical_mesh_->getBoundaryNodeThread();
    for (Thread<Node>::size_type i = 0; i < boundary_node_thread.size(); ++i)
        cmesh->addNode(ComputationalNode::Ptr(new ComputationalNode(boundary_node_thread.getEntityAt(i))));







    return cmesh;
}
