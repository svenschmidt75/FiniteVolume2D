/*
 * Name  : ComputationalMesh
 * Path  : IComputationalMesh
 * Use   : Computational mesh
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "DeclSpec.h"

#include "FiniteVolume2DLib/Mesh.h"
#include "FiniteVolume2DLib/Thread.hpp"
#include "FiniteVolume2DLib/IMeshConnectivity.h"

#include "IComputationalMesh.h"
#include "GeometricalEntityMapper.h"
#include "ComputationalCell.h"
#include "ComputationalFace.h"
#include "ComputationalNode.h"

#include <memory>
#include <unordered_map>


class ComputationalVariableManager;


#pragma warning(disable:4251)
#pragma warning(disable:4275)


class DECL_SYMBOLS_2D ComputationalMesh : public IComputationalMesh {

    friend class ComputationalMeshBuilder;

public:
    typedef std::shared_ptr<ComputationalMesh>       Ptr;
    typedef std::shared_ptr<ComputationalMesh const> CPtr;

public:
    explicit ComputationalMesh(IMeshConnectivity const & mesh_connectivity, std::shared_ptr<ComputationalVariableManager> const & cvar_mgr);

    IMeshConnectivity const &            getMeshConnectivity() const;
    ComputationalVariableManager const & getComputationalVariableManager() const;

    /* Computational entities are stored in threads, so
     * they can easily be queried. Also, this is used
     * to construct the various computational grids for
     * the multigrid method.
     */
    Thread<ComputationalNode> const & getNodeThread(IGeometricEntity::Entity_t entity_type) const;
    Thread<ComputationalFace> const & getFaceThread(IGeometricEntity::Entity_t entity_type) const;
    Thread<ComputationalCell> const & getCellThread() const;

    GeometricalEntityMapper const &   getMapper() const;

    size_t                            getCellIndex(ComputationalCell::Ptr const & ccell) const;

private:
    ComputationalMesh();
    ComputationalMesh(ComputationalMesh const & in);
    ComputationalMesh & operator=(ComputationalMesh const & in);

    // make the non-const access routines private
    Thread<ComputationalNode> & getNodeThread(IGeometricEntity::Entity_t entity_type);
    Thread<ComputationalFace> & getFaceThread(IGeometricEntity::Entity_t entity_type);
    Thread<ComputationalCell> & getCellThread();

    // access only from the ComputationalMeshBuilder
    void addNode(Node::Ptr const & node, ComputationalNode::Ptr const & cnode);
    void addFace(Face::Ptr const & face, ComputationalFace::Ptr const & cface);
    void addCell(Cell::Ptr const & cell, ComputationalCell::Ptr const & ccell);

private:
    std::shared_ptr<ComputationalVariableManager> cvar_mgr_;

    /* The computational entities hold a reference to their geometrical
     * counterparts. The mesh connectivity is given in terms of the
     * geometrical entities. This mapper allows the mapping from
     * geometric entities to computational ones.
     */
    GeometricalEntityMapper   mapper_;

    /* mesh connectivity is inherited from the geometrical mesh */
    IMeshConnectivity const & mesh_connectivity_;

    Thread<ComputationalNode> interior_node_thread_;
    Thread<ComputationalNode> boundary_node_thread_;

    Thread<ComputationalFace> interior_face_thread_;
    Thread<ComputationalFace> boundary_face_thread_;

    Thread<ComputationalCell> cell_thread_;

    /* for mapping ComputationalCells into linear indices */
    std::unordered_map<IGeometricEntity::Id_t, size_t> ccell_index_map_;
};

#pragma warning(default:4275)
#pragma warning(default:4251)
