#include "MeshChecker.h"

#include "IGeometricEntity.h"
#include "Thread.hpp"
#include "Face.h"
#include "Mesh.h"
#include "BoundaryConditionCollection.h"
#include "Util.h"

#include <algorithm>

#include <boost/optional.hpp>
#include <boost/format.hpp>


bool
MeshChecker::checkMesh(Mesh::CPtr const & mesh, BoundaryConditionCollection const & bc) {
    // check that each boundary face has a b.c.
    bool check = true;

    Thread<Face> const & bface_thread = mesh->getFaceThread(IGeometricEntity::BOUNDARY);
    std::for_each(bface_thread.begin(), bface_thread.end(), [&](Face::Ptr const & face) {
        boost::optional<BoundaryConditionCollection::Pair> const & item = bc.find(face->meshId());
        if (!item) {
            boost::format format = boost::format("MeshChecker::checkMesh: No boundary condition specified \
                for boundary face with mesh id %1%!\n") % face->meshId();
            Util::error(format.str());
            check = false;
        }
    });

    if (!check)
        return false;


    // check that no non-boundary face has a b.c.
    check = true;

    Thread<Face> const & iface_thread = mesh->getFaceThread(IGeometricEntity::INTERIOR);
    std::for_each(iface_thread.begin(), iface_thread.end(), [&](Face::Ptr const & face) {
        boost::optional<BoundaryConditionCollection::Pair> const & item = bc.find(face->meshId());
        if (item) {
            boost::format format = boost::format("MeshChecker::checkMesh: Boundary condition specified \
                                                 for internal face with mesh id %1%!\n") % face->meshId();
            Util::error(format.str());
            check = false;
        }
    });

    if (!check)
        return false;


    /* Check that 
     * 1. internal faces have two cell neighbors
     * 2. boundary faces have one cell neighbor
     */
    IMeshConnectivity const & mesh_connectivity = mesh->getMeshConnectivity();

    std::for_each(bface_thread.begin(), bface_thread.end(), [&](Face::Ptr const & face) {
        boost::optional<EntityCollection<Cell>> cell_nbrs = mesh_connectivity.getCellsAttachedToFace(face);

        if (!cell_nbrs) {
            boost::format format = boost::format("MeshChecker::checkMesh: Expected two cell neighbors \
                                                 for internal face with mesh id %1%!\n") % face->meshId();
            Util::error(format.str());
            check = false;
        }

        else if ((*cell_nbrs).size() != 1) {
            boost::format format = boost::format("MeshChecker::checkMesh: Expected two cell neighbors \
                                                 for internal face with mesh id %1%!\n") % face->meshId();
            Util::error(format.str());
            check = false;
        }
    });

    if (!check)
        return false;


    std::for_each(iface_thread.begin(), iface_thread.end(), [&](Face::Ptr const & face) {
        boost::optional<EntityCollection<Cell>> cell_nbrs = mesh_connectivity.getCellsAttachedToFace(face);

        if (!cell_nbrs) {
            boost::format format = boost::format("MeshChecker::checkMesh: Expected two cell neighbors \
                                                 for internal face with mesh id %1%!\n") % face->meshId();
            Util::error(format.str());
            check = false;
        }

        else if ((*cell_nbrs).size() != 2) {
            boost::format format = boost::format("MeshChecker::checkMesh: Expected two cell neighbors \
                                                 for internal face with mesh id %1%!\n") % face->meshId();
            Util::error(format.str());
            check = false;
        }
    });

    return check;
}
