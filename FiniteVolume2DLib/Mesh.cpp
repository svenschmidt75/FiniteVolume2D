#include "Mesh.h"


void
Mesh::addVertex(Vertex::Ptr const & vertex, bool on_boundary) {
    if (on_boundary) {
        Thread<Vertex> & thread = getBoundaryVertexThread();
        thread.insert(vertex);
    }
    else {
        Thread<Vertex> & thread = getInteriorVertexThread();
        thread.insert(vertex);
    }
}

Thread<Vertex> &
Mesh::getBoundaryVertexThread() {
    return boundary_vertex_thread_;
}

Thread<Vertex> &
Mesh::getInteriorVertexThread() {
    return interior_vertex_thread_;
}
