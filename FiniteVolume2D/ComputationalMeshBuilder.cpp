#include "ComputationalMeshBuilder.h"

#include "FluxComputationalMolecule.h"

#include "FiniteVolume2DLib/Util.h"
#include "FiniteVolume2DLib/Thread.hpp"
#include "FiniteVolume2DLib/Node.h"

#include <boost/format.hpp>


ComputationalMeshBuilder::ComputationalMeshBuilder(Mesh::Ptr const & geometrical_mesh, BoundaryConditionCollection const & bc) : geometrical_mesh_(geometrical_mesh), bc_(bc) {}

bool
ComputationalMeshBuilder::addComputationalVariable(std::string const & cell_var, std::string const & flux_var, FluxEvaluator_t const & flux_evaluator) {
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

    insertComputationalEntities(cmesh);
    
    return cmesh;
}

void
ComputationalMeshBuilder::insertComputationalEntities(ComputationalMesh::Ptr & cmesh) const {
    // build computational nodes
    Thread<Node> const & interior_node_thread = geometrical_mesh_->getNodeThread(IGeometricEntity::INTERIOR);
    for (Thread<Node>::size_type i = 0; i < interior_node_thread.size(); ++i)
        cmesh->addNode(interior_node_thread.getEntityAt(i), ComputationalNode::Ptr(new ComputationalNode(interior_node_thread.getEntityAt(i))));

    Thread<Node> const & boundary_node_thread = geometrical_mesh_->getNodeThread(IGeometricEntity::BOUNDARY);
    for (Thread<Node>::size_type i = 0; i < boundary_node_thread.size(); ++i)
        cmesh->addNode(boundary_node_thread.getEntityAt(i), ComputationalNode::Ptr(new ComputationalNode(boundary_node_thread.getEntityAt(i))));


    // extract the computational molecule for faces


    // build computational faces
    Thread<Face> const & interior_face_thread = geometrical_mesh_->getFaceThread(IGeometricEntity::INTERIOR);
    for (Thread<Face>::size_type i = 0; i < interior_face_thread.size(); ++i)
        cmesh->addFace(interior_face_thread.getEntityAt(i), ComputationalFace::Ptr(new ComputationalFace(interior_face_thread.getEntityAt(i))));

    Thread<Face> const & boundary_face_thread = geometrical_mesh_->getFaceThread(IGeometricEntity::BOUNDARY);
    for (Thread<Face>::size_type i = 0; i < boundary_face_thread.size(); ++i)
    {
        BoundaryCondition::Ptr face_bc = NULL;

        Face::Ptr const & face = boundary_face_thread.getEntityAt(i);

        boost::optional<BoundaryConditionCollection::Pair> face_bc_opt = bc_.find(face->id());
        if (face_bc_opt)
            face_bc = BoundaryCondition::Ptr(new BoundaryCondition(*face_bc_opt));

        ComputationalFace::Ptr cface(new ComputationalFace(boundary_face_thread.getEntityAt(i)));
        cface->setBoundaryCondition(face_bc);

        std::for_each(computational_variables_.begin(), computational_variables_.end(), [&](ComputationalVariables_t::value_type const & in) {
            cface->setComputationalMolecule(FluxComputationalMolecule(in.cfluxvar_name));
        });

        cmesh->addFace(boundary_face_thread.getEntityAt(i), cface);
    }

    // build computational cells
    Thread<Cell> const & cell_thread = geometrical_mesh_->getCellThread();
    for (Thread<Cell>::size_type i = 0; i < cell_thread.size(); ++i)
        cmesh->addCell(cell_thread.getEntityAt(i), ComputationalCell::Ptr(new ComputationalCell(cell_thread.getEntityAt(i))));
}
