#include "ComputationalMesh.h"


/* Explicitly generate code for the below specializations.
 * This makes their methods available in the UnitTest
 * project, i.e. the code is exported in the dll.
 */
template class Thread<Node>;
template class Thread<Face>;
template class Thread<Cell>;


ComputationalMesh::ComputationalMesh(IMeshConnectivity const & mesh_connectivity) : mesh_connectivity_(mesh_connectivity) {}

IMeshConnectivity const &
ComputationalMesh::getMeshConnectivity() const {
    return mesh_connectivity_;
}

Thread<ComputationalNode> &
ComputationalMesh::getNodeThread(IGeometricEntity::Entity_t entity_type) {
    if (entity_type == IGeometricEntity::BOUNDARY)
        return boundary_node_thread_;
    return interior_node_thread_;
}

Thread<ComputationalFace> &
ComputationalMesh::getFaceThread(IGeometricEntity::Entity_t entity_type) {
    if (entity_type == IGeometricEntity::BOUNDARY)
        return boundary_face_thread_;
    return interior_face_thread_;
}

Thread<ComputationalCell> &
ComputationalMesh::getCellThread() {
    return cell_thread_;
}

void
ComputationalMesh::addNode(Node::Ptr const & node, ComputationalNode::Ptr const & cnode) {
    Thread<ComputationalNode> & thread = getNodeThread(cnode->getEntityType());
    thread.insert(cnode);

    mapper_.addNode(node, cnode);
}

void
ComputationalMesh::addFace(Face::Ptr const & face, ComputationalFace::Ptr const & cface) {
    Thread<ComputationalFace> & thread = getFaceThread(face->getEntityType());
    thread.insert(cface);

    mapper_.addFace(face, cface);
}

void
ComputationalMesh::addCell(Cell::Ptr const & cell, ComputationalCell::Ptr const & ccell) {
    Thread<ComputationalCell> & thread = getCellThread();
    thread.insert(ccell);

    mapper_.addCell(cell, ccell);
}