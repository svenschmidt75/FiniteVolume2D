#include "ComputationalMeshBuilder.h"

#include "FluxComputationalMolecule.h"
#include "ComputationalMolecule.h"
#include "IComputationalGridAccessor.h"

#include "FiniteVolume2DLib/Util.h"
#include "FiniteVolume2DLib/Thread.hpp"
#include "FiniteVolume2DLib/Node.h"

#include <boost/format.hpp>


ComputationalMeshBuilder::ComputationalMeshBuilder(Mesh::CPtr const & geometrical_mesh, BoundaryConditionCollection const & bc) : geometrical_mesh_(geometrical_mesh), bc_(bc) {}

bool
ComputationalMeshBuilder::addComputationalVariable(std::string const & var_name, FluxEvaluator_t const & flux_evaluator) {
    /* There are two types of variables.
     * 1. The ones that are solved for (active) at the cell-centers and
     * 2. the ones that are user-defined, but are NOT
     * solved for (passive).
     * For example, when the gradient is needed at face vertices,
     * we first compute the gradient at cell centers in terms of
     * the variables of type 1 (i.e. the ones at the cell centers
     * that we will solve for). The corresponding comp. molecule
     * is added to the computational cell, but it will play no role
     * in solving the linear system. When computing the face fluxes,
     * we can access the cell-centered gradient molecules to compute
     * the face vertex gradients using the computational molecule for
     * them.
     * 
     *       var_name: Variable to solve for at each cell center.
     * flux_evaluator: Callback, called for each cell face to compute
     *                 flux through face.
     */
    return cvar_mgr_.registerVariable(var_name, flux_evaluator);
}

namespace {

    template<typename VAR_TYPE, typename DATA_TYPE>
    bool
    add(VAR_TYPE const & vars, DATA_TYPE & data, std::string const & var_name) {
        /* Add a user-defined variable for nodes. These variables will not
         * be solved for, only computational molecules will be added to
         * the computational nodes created.
         */

        // check that there is no active variables with the same name
        if (vars.getBaseIndex(var_name) >= 0) {
            boost::format format = boost::format("ComputationalMeshBuilder::addPassiveComputationalNodeVariable: User-defined node variable \
                                                 %1% cannot have the same name as computational variable!\n") % var_name;
            Util::error(format.str());
            return false;
        }


        // check that there is no active variables with the same name
        auto it = data.find(var_name);
        if (it != data.end()) {
            boost::format format = boost::format("ComputationalMeshBuilder::addPassiveComputationalNodeVariable: User-defined node variable \
                                                 %1% already added!\n") % var_name;
            Util::error(format.str());
            return false;
        }

        data.insert(var_name);

        return true;
    }

}

bool
ComputationalMeshBuilder::addPassiveComputationalNodeVariable(std::string const & var_name) {
    /* Add a user-defined variable for nodes. These variables will not
     * be solved for, only computational molecules will be added to
     * the computational nodes created.
     */
    return add(cvar_mgr_, node_vars_, var_name);
}

bool
ComputationalMeshBuilder::addPassiveComputationalFaceVariable(std::string const & var_name) {
    /* Add a user-defined variable for faces. These variables will not
     * be solved for, only computational molecules will be added to
     * the computational faces created.
     */
    return add(cvar_mgr_, face_vars_, var_name);
}

bool
ComputationalMeshBuilder::addPassiveComputationalCellVariable(std::string const & var_name) {
    /* Add a user-defined variable for nodes. These variables will not
     * be solved for, only computational molecules will be added to
     * the computational nodes created.
     */
    return add(cvar_mgr_, cell_vars_, var_name);
}

ComputationalMesh::Ptr
ComputationalMeshBuilder::build() const {
    if (cvar_mgr_.size() == 0) {
        boost::format format = boost::format("ComputationalMeshBuilder::build: No computational variables defined!\n");
        Util::error(format.str());
        return std::nullptr_t();
    }

    ComputationalMesh::Ptr cmesh(new ComputationalMesh(geometrical_mesh_->getMeshConnectivity()));

    /* Create ComputationalNodes, ComputationalFaces and ComputationalCells.
     * Also, insert computational variables and boundary conditions.
     */
    insertComputationalEntities(cmesh);
    
    // compute the face fluxes
    computeFaceFluxes(cmesh);

    return cmesh;
}

void
ComputationalMeshBuilder::insertComputationalEntities(ComputationalMesh::Ptr & cmesh) const {
    GeometricalEntityMapper const & mapper = cmesh->getMapper();

    /*
     * Build computational nodes
     */

    Thread<Node> const & interior_node_thread = geometrical_mesh_->getNodeThread(IGeometricEntity::INTERIOR);
    for (Thread<Node>::size_type i = 0; i < interior_node_thread.size(); ++i) {
        Node::Ptr const & node = interior_node_thread.getEntityAt(i);
        ComputationalNode::Ptr & cnode = std::make_shared<ComputationalNode>(node);

        // set all applicable (passive) computational variables
        setComputationalVariables(cnode);

        cmesh->addNode(node, cnode);
    }

    Thread<Node> const & boundary_node_thread = geometrical_mesh_->getNodeThread(IGeometricEntity::BOUNDARY);
    for (Thread<Node>::size_type i = 0; i < boundary_node_thread.size(); ++i) {
        Node::Ptr const & node = boundary_node_thread.getEntityAt(i);
        ComputationalNode::Ptr & cnode = std::make_shared<ComputationalNode>(node);

        // set all applicable (passive) computational variables
        setComputationalVariables(cnode);

        cmesh->addNode(node, cnode);
    }


    /*
     * Build computational faces
     */

    Thread<Face> const & interior_face_thread = geometrical_mesh_->getFaceThread(IGeometricEntity::INTERIOR);
    for (Thread<Face>::size_type i = 0; i < interior_face_thread.size(); ++i) {
        Face::Ptr const & face = interior_face_thread.getEntityAt(i);

        // convert geometric nodes into computational nodes
        EntityCollection<Node> const & nodes = face->getNodes();
        EntityCollection<ComputationalNode> cnodes;

        std::for_each(nodes.begin(), nodes.end(), [&](Node::Ptr const & node) {
            ComputationalNode::Ptr const & cnode = mapper.getComputationalNode(node);
            assert(cnode != std::nullptr_t());
            cnodes.insert(cnode);
        });

        ComputationalFace::Ptr & cface = ComputationalFace::Ptr(new ComputationalFace(face, cnodes));

        // set all applicable (passive) computational variables
        setComputationalVariables(cface);

        cmesh->addFace(face, cface);
    }

    Thread<Face> const & boundary_face_thread = geometrical_mesh_->getFaceThread(IGeometricEntity::BOUNDARY);
    for (Thread<Face>::size_type i = 0; i < boundary_face_thread.size(); ++i)
    {
        BoundaryCondition::Ptr face_bc = nullptr;

        Face::Ptr const & face = boundary_face_thread.getEntityAt(i);


        // convert geometric nodes into computational nodes
        EntityCollection<Node> const & nodes = face->getNodes();
        EntityCollection<ComputationalNode> cnodes;

        std::for_each(nodes.begin(), nodes.end(), [&](Node::Ptr const & node) {
            ComputationalNode::Ptr const & cnode = mapper.getComputationalNode(node);
            assert(cnode != std::nullptr_t());
            cnodes.insert(cnode);
        });

        ComputationalFace::Ptr cface(new ComputationalFace(face, cnodes));


        // extract boundary conditions
        boost::optional<BoundaryConditionCollection::Pair> face_bc_opt = bc_.find(face->meshId());
        if (face_bc_opt)
            face_bc = BoundaryCondition::Ptr(new BoundaryCondition(*face_bc_opt));

        cface->setBoundaryCondition(face_bc);

        // set all applicable (passive) computational variables
        setComputationalVariables(cface);

        cmesh->addFace(face, cface);
    }


    /*
     * Build computational cells
     */

    Thread<Cell> const & cell_thread = geometrical_mesh_->getCellThread();
    for (Thread<Cell>::size_type i = 0; i < cell_thread.size(); ++i) {
        Cell::Ptr const & cell = cell_thread.getEntityAt(i);

        // convert geometric faces into computational faces
        EntityCollection<Face> const & faces = cell->getFaces();
        EntityCollection<ComputationalFace> cfaces;

        std::for_each(faces.begin(), faces.end(), [&](Face::Ptr const & face) {
            ComputationalFace::Ptr const & cface = mapper.getComputationalFace(face);
            assert(cface != std::nullptr_t());
            cfaces.insert(cface);
        });

        ComputationalCell::Ptr ccell(new ComputationalCell(cell, cfaces));

        // set all applicable computational variables
        setComputationalVariables(ccell);

        cmesh->addCell(cell_thread.getEntityAt(i), ccell);
    }
}

void
ComputationalMeshBuilder::setComputationalVariables(ComputationalNode::Ptr & cnode) const {
    /* All variables solved for are located at the cell-centers.
     * Nodes cannot have variables, but ComputationalMolecules, that the user
     * can use to do additional computations etc. These ComputationalMolecules
     * must only reference variables at cell centers that have been registered
     * with "addComputationalVariable".
     */
    std::for_each(node_vars_.begin(), node_vars_.end(), [&](PassiveNodeVars_t::value_type const & var_name) {
        cnode->addComputationalMolecule(var_name);
    });
}

void
ComputationalMeshBuilder::setComputationalVariables(ComputationalFace::Ptr & cface) const {
    /* All variables solved for are located at the cell-centers.
     * Faces cannot have variables, but ComputationalMolecules, that the user
     * can use to do additional computations etc. These ComputationalMolecules
     * must only reference variables at cell centers that have been registered
     * with "addComputationalVariable".
     */

    /*
     * Add the cell-centered variables to the faces.
     * This is done because the face fluxes are integral
     * to the finite volume method. 
     * We first compute the face fluxes as they are needed to fill in
     * the ComputationalMolecules of the cell-centered variables.
     */

    ComputationalVariableManager::Iterator_t it = cvar_mgr_.begin();
    ComputationalVariableManager::Iterator_t it_end = cvar_mgr_.end();

    for (; it != it_end; ++it) {
        cface->addComputationalMolecule(it->name);
    }


    // add the user-defined face variables

    std::for_each(face_vars_.begin(), face_vars_.end(), [&](PassiveFaceVars_t::value_type const & var_name) {
        cface->addComputationalMolecule(var_name);
    });
}

bool
ComputationalMeshBuilder::setComputationalVariables(ComputationalCell::Ptr & ccell) const {
    /* For computational cells, we need to add the cell-centered variables that
     * will be solved for and that were registered via "addComputationalVariable"
     * and also the user-defined variables that will NOT be solved for.
     */


    /*
     * Add the cell-centered variables
     */

    ComputationalVariableManager::Iterator_t it = cvar_mgr_.begin();
    ComputationalVariableManager::Iterator_t it_end = cvar_mgr_.end();

    for (; it != it_end; ++it) {
        // create and add the comp. variable to the comp. cell
        if (!cvar_mgr_.create(ccell, it->name)) {
            boost::format format = boost::format("ComputationalMeshBuilder::setComputationalVariables: Error creating computational variable \
                                                 %1% on cell with cell id %2%!\n") % it->name % ccell->id();
            Util::error(format.str());
            return false;
        }
    }


    /*
     * Add all user-defined cell variables
     */
    std::for_each(cell_vars_.begin(), cell_vars_.end(), [&](PassiveCellVars_t::value_type const & var_name) {
        ccell->addComputationalMolecule(var_name);
    });

    return true;
}

void
ComputationalMeshBuilder::computeFaceFluxes(ComputationalMesh::Ptr & cmesh) const {
    /* For all cells, fill in the ComputationalMolecules for the
     * ComputationalVariables.
     */
    IComputationalGridAccessor grid_accessor(cmesh->getMeshConnectivity(), cmesh->getMapper());

    Thread<ComputationalCell> const & cell_thread = cmesh->getCellThread();
    for (Thread<ComputationalCell>::size_type i = 0; i < cell_thread.size(); ++i) {
        ComputationalCell::Ptr const & ccell = cell_thread.getEntityAt(i);

        EntityCollection<ComputationalFace> const & cfaces = ccell->getComputationalFaces();
        std::for_each(cfaces.begin(), cfaces.end(), [&](ComputationalFace::Ptr const & cface) {
            /* Compute the face flux through all cell faces
             * for all cell-centered variables. Each such
             * variable should have its own flux-evaluator.
             */ 
            ComputationalVariableManager::Iterator_t it     = cvar_mgr_.begin();
            ComputationalVariableManager::Iterator_t it_end = cvar_mgr_.end();

            for (; it != it_end; ++it) {
                // compute flux through face
                (it->flux_eval)(grid_accessor, ccell, cface);
            }
        });
    }
}
