/*
 * Name  : ComputationalMeshBuilder
 * Path  : 
 * Use   : Builds a computational mesh from a geometrical mesh.
 *         Nodes -> ComputationalNodes
 *         Faces -> ComputationalFaces
 *         Cells -> ComputationalCells
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "DeclSpec.h"

#include "ComputationalCell.h"
#include "ComputationalFace.h"
#include "ComputationalVariableManager.h"

#include "FiniteVolume2DLib/Mesh.h"
#include "FiniteVolume2DLib/BoundaryConditionCollection.h"

#include "ComputationalMesh.h"

#include <functional>
#include <deque>


class ComputationalGridAccessor;


#pragma warning(disable:4251)


class DECL_SYMBOLS_2D ComputationalMeshBuilder {
public:
    typedef ComputationalVariableManager::FluxEvaluator_t FluxEvaluator_t;

public:
    explicit ComputationalMeshBuilder(Mesh::Ptr const & mesh, BoundaryConditionCollection const & bc);

    bool                   addComputationalVariable(std::string const & cell_var, FluxEvaluator_t const & flux_evaluator);
    ComputationalMesh::Ptr build() const;

private:
    void insertComputationalEntities(ComputationalMesh::Ptr & cmesh) const;
    void computeFaceFluxes(ComputationalMesh::Ptr & cmesh) const;
    void setComputationalVariables(ComputationalNode::Ptr & cnode) const;
    void setComputationalVariables(ComputationalFace::Ptr & cface) const;
    void setComputationalVariables(ComputationalCell::Ptr & ccell) const;

private:
    // geometric mesh to convert to a computational one
    Mesh::Ptr geometrical_mesh_;

    // all face boundary conditions
    BoundaryConditionCollection const & bc_;

    // registered variables to solve for
    ComputationalVariableManager cvar_mgr_;
};

#pragma warning(default:4251)
