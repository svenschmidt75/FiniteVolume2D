#include "ComputationalMeshBuilder.h"

#include "FluxComputationalMolecule.h"
#include "ComputationalMolecule.h"

#include "FiniteVolume2DLib/Util.h"
#include "FiniteVolume2DLib/Thread.hpp"
#include "FiniteVolume2DLib/Node.h"

#include <boost/format.hpp>


ComputationalMeshBuilder::ComputationalMeshBuilder(Mesh::Ptr const & geometrical_mesh, BoundaryConditionCollection const & bc) : geometrical_mesh_(geometrical_mesh), bc_(bc) {}

bool
ComputationalMeshBuilder::addComputationalVariable(std::string const & cell_var, std::string const & flux_var, FluxEvaluator_t const & flux_evaluator) {
    /* There are two types of variables.
     * 1. The ones that are solved for and
     * 2. the ones that are used for caching values, but are NOT
     * solved for.
     * For example, when the gradient is needed at face vertices,
     * we first compute the gradient at cell centers in terms of
     * the variables of type 1 (i.e. the ones that we will solve
     * for). The corresponding comp. molecule is added to the
     * computational cell, but it will play no role in solving the
     * linear system. When computing the face fluxes, we can access
     * the cell-centered gradient molecules to compute the face vertex
     * gradients using the computational molecule for them.
     * 
     * cell_val: Variable to solve for at each cell center.
     * flux_var: Corresponding flux variable (for diffusion terms)
     * flux_evaluator: Callback, called for each cell face to compute
     *                 flux through face.
     */

    auto it = std::find_if(computational_variables_.begin(), computational_variables_.end(), [&](ComputationalVariables_t::value_type const & in) -> bool {
        return in.cvar_name == cell_var;
    });

    if (it != computational_variables_.end()) {
        boost::format format = boost::format("ComputationalMeshBuilder::addComputationalVariable: Variable %1% already defined!\n") % cell_var;
        Util::error(format.str());
        return false;
    }

    it = std::find_if(computational_variables_.begin(), computational_variables_.end(), [&](ComputationalVariables_t::value_type const & in) -> bool {
        return in.cfluxvar_name == flux_var;
    });

    if (it != computational_variables_.end()) {
        boost::format format = boost::format("ComputationalMeshBuilder::addComputationalVariable: Flux variable %1% already added for computational variable %2%!\n") % cell_var % it->cvar_name;
        Util::error(format.str());
        return false;
    }

    /* This only registers the comp. variable and the flux callback.
     * We will insert the comp. variables into the cells in
     * the build() method.
     */
    computational_variables_.push_back(ComputationalVariables(cell_var, flux_var, flux_evaluator));

    return true;
}


ComputationalMesh::Ptr
ComputationalMeshBuilder::build() const {
    if (computational_variables_.empty()) {
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

    // build computational nodes
    Thread<Node> const & interior_node_thread = geometrical_mesh_->getNodeThread(IGeometricEntity::INTERIOR);
    for (Thread<Node>::size_type i = 0; i < interior_node_thread.size(); ++i)
        cmesh->addNode(interior_node_thread.getEntityAt(i), ComputationalNode::Ptr(new ComputationalNode(interior_node_thread.getEntityAt(i))));

    Thread<Node> const & boundary_node_thread = geometrical_mesh_->getNodeThread(IGeometricEntity::BOUNDARY);
    for (Thread<Node>::size_type i = 0; i < boundary_node_thread.size(); ++i)
        cmesh->addNode(boundary_node_thread.getEntityAt(i), ComputationalNode::Ptr(new ComputationalNode(boundary_node_thread.getEntityAt(i))));


    // build computational faces
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

        cmesh->addFace(face, ComputationalFace::Ptr(new ComputationalFace(interior_face_thread.getEntityAt(i), cnodes)));
    }

    Thread<Face> const & boundary_face_thread = geometrical_mesh_->getFaceThread(IGeometricEntity::BOUNDARY);
    for (Thread<Face>::size_type i = 0; i < boundary_face_thread.size(); ++i)
    {
        BoundaryCondition::Ptr face_bc = std::nullptr_t();

        Face::Ptr const & face = boundary_face_thread.getEntityAt(i);


        // convert geometric nodes into computational nodes
        EntityCollection<Node> const & nodes = face->getNodes();
        EntityCollection<ComputationalNode> cnodes;

        std::for_each(nodes.begin(), nodes.end(), [&](Node::Ptr const & node) {
            ComputationalNode::Ptr const & cnode = mapper.getComputationalNode(node);
            assert(cnode != std::nullptr_t());
            cnodes.insert(cnode);
        });


        boost::optional<BoundaryConditionCollection::Pair> face_bc_opt = bc_.find(face->meshId());
        if (face_bc_opt)
            face_bc = BoundaryCondition::Ptr(new BoundaryCondition(*face_bc_opt));

        ComputationalFace::Ptr cface(new ComputationalFace(boundary_face_thread.getEntityAt(i), cnodes));
        cface->setBoundaryCondition(face_bc);


        // insert the computational molecules into the face
//         std::for_each(computational_variables_.begin(), computational_variables_.end(), [&](ComputationalVariables_t::value_type const & in) {
//             cface->setComputationalMolecule(FluxComputationalMolecule(in.cfluxvar_name));
//         });

        cmesh->addFace(boundary_face_thread.getEntityAt(i), cface);
    }


    // build computational cells
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


        // insert the computational molecules into the cell
//         std::for_each(computational_variables_.begin(), computational_variables_.end(), [&](ComputationalVariables_t::value_type const & in) {
//             ccell->setComputationalMolecule(ComputationalMolecule(in.cvar_name));
//         });

        cmesh->addCell(cell_thread.getEntityAt(i), ccell);
    }
}

void
ComputationalMeshBuilder::computeFaceFluxes(ComputationalMesh::Ptr & cmesh) const {
    Thread<ComputationalCell> const & cell_thread = cmesh->getCellThread();
    for (Thread<ComputationalCell>::size_type i = 0; i < cell_thread.size(); ++i) {
        ComputationalCell::Ptr const & ccell = cell_thread.getEntityAt(i);

        EntityCollection<ComputationalFace> const & cfaces = ccell->getComputationalFaces();
        std::for_each(cfaces.begin(), cfaces.end(), [&](ComputationalFace::Ptr const & cface) {
            // compute flux through face
//            flux_eval(ccell, cface);
        });
    }
}
