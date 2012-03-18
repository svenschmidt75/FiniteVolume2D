/*
 * Name  : Mesh
 * Path  : IMesh
 * Use   : Mesh class
 * Author: Sven Schmidt
 * Date  : 03/03/2012
 */
#pragma once

#include "IMesh.h"

#include "DeclSpec.h"
#include "Vertex.h"
#include "Thread.hpp"
#include "MeshConnectivity.h"


#pragma warning(disable:4251)


class DECL_SYMBOLS Mesh : public IMesh {
public:
    typedef std::shared_ptr<Mesh> Ptr;

public:
    // FROM IMesh
    void addVertex(Vertex::Ptr const & vertex);
    void addFace(Face::Ptr const & face);
    void addCell(Cell::Ptr const & cell);

    IMeshConnectivity const & getMeshConnectivity() const;


    Thread<Vertex> & getBoundaryVertexThread();
    Thread<Vertex> & getInteriorVertexThread();

    Thread<Face> & getBoundaryFaceThread();
    Thread<Face> & getInteriorFaceThread();

    Thread<Cell> & getCellThread();

    static Ptr create();

private:
    Mesh();

private:
    Thread<Vertex> interior_vertex_thread_;
    Thread<Vertex> boundary_vertex_thread_;

    Thread<Face> interior_face_thread_;
    Thread<Face> boundary_face_thread_;

    Thread<Cell> cell_thread_;

    MeshConnectivity mesh_connectivity_;
};

#pragma warning(default:4251)
