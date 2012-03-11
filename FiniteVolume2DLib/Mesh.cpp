#include "Mesh.h"


void
Mesh::addVertex(Vertex::Ptr const & vertex) {
    if (vertex->onBoundary()) {
        Thread<Vertex> & thread = getBoundaryVertexThread();
        thread.insert(vertex);
    }
    else {
        Thread<Vertex> & thread = getInteriorVertexThread();
        thread.insert(vertex);
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

Thread<Vertex> &
Mesh::getBoundaryVertexThread() {
    return boundary_vertex_thread_;
}

Thread<Vertex> &
Mesh::getInteriorVertexThread() {
    return interior_vertex_thread_;
}

Thread<Face> &
Mesh::getBoundaryFaceThread() {
    return boundary_face_thread_;
}

Thread<Face> &
Mesh::getInteriorFaceThread() {
    return interior_face_thread_;
}
