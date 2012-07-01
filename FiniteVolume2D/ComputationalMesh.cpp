#include "ComputationalMesh.h"

#include "ComputationalVariableManager.h"
#include "ComputationalMeshSolverHelper.h"

#include "FiniteVolume2DLib/Util.h"

#include <cassert>

#include <boost/format.hpp>


/* Explicitly generate code for the below specializations.
 * This makes their methods available in the UnitTest
 * project, i.e. the code is exported in the dll.
 */
template class Thread<Node>;
template class Thread<Face>;
template class Thread<Cell>;


ComputationalMesh::ComputationalMesh(IMeshConnectivity const & mesh_connectivity, ComputationalVariableManager::Ptr const & cvar_mgr)
    :
    mesh_connectivity_(mesh_connectivity),
    cvar_mgr_(cvar_mgr) {}

IMeshConnectivity const &
ComputationalMesh::getMeshConnectivity() const {
    return mesh_connectivity_;
}

ComputationalVariableManager const &
ComputationalMesh::getComputationalVariableManager() const {
    return *cvar_mgr_;
}

Thread<ComputationalNode> const &
ComputationalMesh::getNodeThread(IGeometricEntity::Entity_t entity_type) const {
    if (entity_type == IGeometricEntity::BOUNDARY)
        return boundary_node_thread_;
    return interior_node_thread_;
}

Thread<ComputationalNode> &
ComputationalMesh::getNodeThread(IGeometricEntity::Entity_t entity_type) {
    if (entity_type == IGeometricEntity::BOUNDARY)
        return boundary_node_thread_;
    return interior_node_thread_;
}

Thread<ComputationalFace> const &
ComputationalMesh::getFaceThread(IGeometricEntity::Entity_t entity_type) const {
    if (entity_type == IGeometricEntity::BOUNDARY)
        return boundary_face_thread_;
    return interior_face_thread_;
}

Thread<ComputationalFace> &
ComputationalMesh::getFaceThread(IGeometricEntity::Entity_t entity_type) {
    if (entity_type == IGeometricEntity::BOUNDARY)
        return boundary_face_thread_;
    return interior_face_thread_;
}

Thread<ComputationalCell> const &
ComputationalMesh::getCellThread() const {
    return cell_thread_;
}

Thread<ComputationalCell> &
ComputationalMesh::getCellThread() {
    return cell_thread_;
}

GeometricalEntityMapper const &
ComputationalMesh::getMapper() const {
    return mapper_;
}

size_t
ComputationalMesh::getCellIndex(ComputationalCell::Ptr const & ccell) const {
    IGeometricEntity::Id_t cell_id = ccell->id();

    /* We cannot use ccell_index_map_[cell_id].
     * This is because this is a const method,
     * ccell_index_map_ is const as well.
     * There is no const version of operator[]
     * on std::unordered_map because if the
     * element is not there, it will be created,
     * hence it must be non-const.
     */
//    size_t index = ccell_index_map_[cell_id];
    size_t index = ccell_index_map_.at(cell_id);
    return index;
}

bool
ComputationalMesh::setSolution(unsigned int cell_index, unsigned int cvar_index, double value) const {
    /* Insert a solution of the linear system into the ComputationalCell
     * for the ComputationaVariable cvar_index.
     */

    // ComputationalMesh is logically const
    ComputationalMesh* this_ = const_cast<ComputationalMesh*>(this);

    ComputationalCell::Ptr const & ccell = this_->getCellThread().getEntityAt(cell_index);
    if (ccell == nullptr) {
        boost::format format = boost::format("ComputationalMesh::setSolution: ComputationalCell with index %1% not found!\n") % cell_index;
        Util::error(format.str());
        return false;

    }

    ComputationalVariable::Ptr const & cvar = this_->cvar_mgr_->getComputationalVariable(cvar_index);
    if (cvar == nullptr) {
        boost::format format = boost::format("ComputationalMesh::setSolution: ComputationalVariable with index %1% not found!\n") % cvar_index;
        Util::error(format.str());
        return false;
    }

    std::string const & cvar_name = cvar->getName();
    ComputationalMolecule & cm = ccell->getComputationalMolecule(cvar_name);

    cm.setValue(value);

    return true;
}

void
ComputationalMesh::solve() const {
    ComputationalMeshSolverHelper helper(*this);
    helper.solve();
}

void
ComputationalMesh::addNode(Node::Ptr const & node, ComputationalNode::Ptr const & cnode) {
    Thread<ComputationalNode> & thread = getNodeThread(cnode->getEntityType());
    thread.insert(cnode);

    mapper_.addNode(node, cnode);
}

void
ComputationalMesh::addFace(Face::Ptr const & face, ComputationalFace::Ptr const & cface) {
    Thread<ComputationalFace> & thread = getFaceThread(face->getEntityType());
    thread.insert(cface);

    mapper_.addFace(face, cface);
}

void
ComputationalMesh::addCell(Cell::Ptr const & cell, ComputationalCell::Ptr const & ccell) {
    Thread<ComputationalCell> & thread = getCellThread();
    Thread<ComputationalCell>::size_type cell_index = thread.insert(ccell);

    /* Store the linear index of this ComputationalCell. This will be
     * needed when the matrix for the linear system is populated in
     * ComputationalMeshSolverHelper.
     */
    ccell_index_map_[ccell->id()] = cell_index;

    mapper_.addCell(cell, ccell);
}
