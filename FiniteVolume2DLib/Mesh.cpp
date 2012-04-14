#include "Mesh.h"


/* Explicitly generate code for the below specializations.
 * This makes their methods available in the UnitTest
 * project, i.e. the code is exported in the dll.
 */
template class Thread<Node>;
template class Thread<Face>;
template class Thread<Cell>;


Mesh::Mesh() {}

void
Mesh::addNode(Node::Ptr const & node) {
    Thread<Node> & thread = getNodeThread(node->getEntityType());
    thread.insert(node);
}

void
Mesh::addFace(Face::Ptr const & face) {
    Thread<Face> & thread = getFaceThread(face->getEntityType());
    thread.insert(face);

    mesh_connectivity_.insert(face);
}

void
Mesh::addCell(Cell::Ptr const & cell) {
    Thread<Cell> & thread = getCellThread();
    thread.insert(cell);

    mesh_connectivity_.insert(cell);
}

IMeshConnectivity const &
Mesh::getMeshConnectivity() const {
    return mesh_connectivity_;
}

Thread<Node> &
Mesh::getNodeThread(IGeometricEntity::Entity_t entity_type) {
    if (entity_type == IGeometricEntity::BOUNDARY)
        return boundary_node_thread_;
    return interior_node_thread_;
}

Thread<Face> &
Mesh::getFaceThread(IGeometricEntity::Entity_t entity_type) {
    if (entity_type == IGeometricEntity::BOUNDARY)
        return boundary_face_thread_;
    return interior_face_thread_;
}

Thread<Cell> &
Mesh::getCellThread() {
    return cell_thread_;
}

Mesh::Ptr
Mesh::create() {
    return Ptr(new Mesh);
}
