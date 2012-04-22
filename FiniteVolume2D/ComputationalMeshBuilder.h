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

#include "FiniteVolume2D/ComputationalCell.h"
#include "FiniteVolume2D/ComputationalFace.h"

#include "FiniteVolume2DLib/Mesh.h"
#include "FiniteVolume2DLib/BoundaryConditionCollection.h"

#include "ComputationalMesh.h"

#include <functional>
#include <deque>


class ComputationalGridAccessor;


#pragma warning(disable:4251)


class DECL_SYMBOLS_2D ComputationalMeshBuilder {
public:
    typedef std::function<bool (ComputationalGridAccessor const & cgrid, ComputationalCell::Ptr const & cell, ComputationalFace::Ptr & face)> FluxEvaluator_t;

public:
    explicit ComputationalMeshBuilder(Mesh::Ptr const & mesh, BoundaryConditionCollection const & bc);

    bool                   addComputationalVariable(std::string const & cell_var, std::string const & flux_var, FluxEvaluator_t const & flux_evaluator);
    ComputationalMesh::Ptr build() const;

private:
    struct ComputationalVariables {
        explicit ComputationalVariables(std::string const & cvar, std::string const & fvar, FluxEvaluator_t const & flux_eval) : cvar_name(cvar), cfluxvar_name(fvar), flux_evaluator(flux_eval) {}

        // Variable to solve for. One per computational cell center.
        std::string cvar_name;

        // corresponding flux variable
        std::string cfluxvar_name;

        // flux evaluator
        FluxEvaluator_t flux_evaluator;
    };

    typedef std::deque<ComputationalVariables> ComputationalVariables_t;

private:
    void insertComputationalEntities(ComputationalMesh::Ptr & cmesh) const;

private:
    // geometric mesh to convert to a computational one
    Mesh::Ptr geometrical_mesh_;

    // all face boundary conditions
    BoundaryConditionCollection const & bc_;

    // registered variables to solve for
    ComputationalVariables_t computational_variables_;
};

#pragma warning(default:4251)
