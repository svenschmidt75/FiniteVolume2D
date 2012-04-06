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
    if (node->onBoundary()) {
        Thread<Node> & thread = getBoundaryVertexThread();
        thread.insert(node);
    }
    else {
        Thread<Node> & thread = getInteriorVertexThread();
        thread.insert(node);
    }
}

void
Mesh::addFace(Face::Ptr const & face) {
    if (face->onBoundary()) {
        Thread<Face> & thread = getBoundaryFaceThread();
        thread.insert(face);
    }
    else {
        Thread<Face> & thread = getInteriorFaceThread();
        thread.insert(face);
    }

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
Mesh::getBoundaryVertexThread() {
    return boundary_node_thread_;
}

Thread<Node> &
Mesh::getInteriorVertexThread() {
    return interior_node_thread_;
}

Thread<Face> &
Mesh::getBoundaryFaceThread() {
    return boundary_face_thread_;
}

Thread<Face> &
Mesh::getInteriorFaceThread() {
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
