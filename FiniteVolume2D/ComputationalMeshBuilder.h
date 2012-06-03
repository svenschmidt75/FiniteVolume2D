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
#include <set>


#pragma warning(disable:4251)


class DECL_SYMBOLS_2D ComputationalMeshBuilder {
public:
    typedef ComputationalVariableManager::FluxEvaluator_t FluxEvaluator_t;

public:
    explicit ComputationalMeshBuilder(Mesh::CPtr const & mesh, BoundaryConditionCollection const & bc);

    bool                   addComputationalVariable(std::string const & var_name, FluxEvaluator_t const & flux_evaluator);
    bool                   addPassiveComputationalNodeVariable(std::string const & var_name);
    bool                   addPassiveComputationalFaceVariable(std::string const & var_name);
    bool                   addPassiveComputationalCellVariable(std::string const & var_name);
    ComputationalMesh::Ptr build() const;

private:
    void insertComputationalEntities(ComputationalMesh::Ptr & cmesh) const;
    void computeFaceFluxes(ComputationalMesh::Ptr & cmesh) const;
    void setComputationalVariables(ComputationalNode::Ptr & cnode) const;
    void setComputationalVariables(ComputationalFace::Ptr & cface) const;
    bool setComputationalVariables(ComputationalCell::Ptr & ccell) const;

private:
    typedef std::set<std::string> PassiveNodeVars_t;
    typedef std::set<std::string> PassiveFaceVars_t;
    typedef std::set<std::string> PassiveCellVars_t;


private:
    // geometric mesh to convert to a computational one
    Mesh::CPtr const                      geometrical_mesh_;

    // all face boundary conditions
    BoundaryConditionCollection const &   bc_;

    // all variables to solve for
    mutable ComputationalVariableManager  cvar_mgr_;

    // user-defined node/face/cell variables
    PassiveNodeVars_t                     node_vars_;
    PassiveFaceVars_t                     face_vars_;
    PassiveCellVars_t                     cell_vars_;
};

#pragma warning(default:4251)
