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
#include "Node.h"
#include "Thread.hpp"
#include "MeshConnectivity.h"


#pragma warning(disable:4251)


class DECL_SYMBOLS_2DLIB Mesh : public IMesh {
public:
    typedef std::shared_ptr<Mesh>       Ptr;
    typedef std::shared_ptr<Mesh const> CPtr;

public:
    // FROM IMesh
    void addNode(Node::CPtr const & node);
    void addFace(Face::CPtr const & face);
    void addCell(Cell::CPtr const & cell);


    IMeshConnectivity const & getMeshConnectivity() const;

    // all entities are kept in threads
    Thread<Node const> const & getNodeThread(IGeometricEntity::Entity_t type) const;
    Thread<Face const> const & getFaceThread(IGeometricEntity::Entity_t type) const;
    Thread<Cell const> const & getCellThread() const;

    static Ptr create();

private:
    Mesh();
    Mesh(Mesh const & in);

    // make the mutable ones private so the entities can
    // only be querried from outside
    Thread<Node> & getNodeThread(IGeometricEntity::Entity_t type);
    Thread<Face> & getFaceThread(IGeometricEntity::Entity_t type);
    Thread<Cell> & getCellThread();

private:
    Thread<Node> interior_node_thread_;
    Thread<Node> boundary_node_thread_;

    Thread<Face> interior_face_thread_;
    Thread<Face> boundary_face_thread_;

    Thread<Cell> cell_thread_;

    MeshConnectivity mesh_connectivity_;
};

#pragma warning(default:4251)
