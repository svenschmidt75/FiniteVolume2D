/*
 * Name  : Mesh
 * Path  : IMesh
 * Use   : Mesh class
 * Author: Sven Schmidt
 * Date  : 03/03/2012
 */
#pragma once

#include "IMesh.h"

#include "Vertex.h"
#include "Thread.hpp"
#include "MeshConnectivity.h"

#include <boost/shared_ptr.hpp>


class Mesh : public IMesh {
public:
    typedef boost::shared_ptr<Mesh> Ptr;

public:
    // FROM IMesh
    void addVertex(Vertex::Ptr const & vertex, bool on_boundary);

    Thread<Vertex> & getBoundaryVertexThread();
    Thread<Vertex> & getInteriorVertexThread();

private:
    Thread<Vertex> interior_vertex_thread_;
    Thread<Vertex> boundary_vertex_thread_;

    MeshConnectivity mesh_connectivity_;
};
