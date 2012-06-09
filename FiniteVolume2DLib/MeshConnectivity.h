/*
 * Name  : MeshConnectivity
 * Path  : 
 * Use   : Contains mesh connectivity, like
 *         - neighbor nodes of a node
 *         - faces attached to a node
 *         - cells attached to a node
 *         - cells attached to a face
 *         
 *         Each entity knows about the "smaller" entities it is
 *         made up of. For example, a cell knows about the faces
 *         it is made up of etc, but a face does not know which
 *         cells are attached to it, i.e. this object answers
 *         questions from bottom up.
 * Author: Sven Schmidt
 * Date  : 03/10/2012
 */
#pragma once

#include "IMeshConnectivity.h"
#include "Node.h"
#include "Face.h"
#include "Cell.h"
#include "EntityCollection.hpp"
#include "NodeConnectivity.h"
#include "FaceConnectivity.h"

#include <map>
#include <boost/optional.hpp>


class MeshConnectivity : public IMeshConnectivity {
public:
    // FROM IMeshConnectivity
    void                                          insert(Face::CPtr const & face);
    void                                          insert(Cell::CPtr const & cell);
    boost::optional<EntityCollection<Node const>> getNodeNeighbors(Node::CPtr const & node) const;
    boost::optional<EntityCollection<Face const>> getFacesAttachedToNode(Node::CPtr const & node) const;
    boost::optional<EntityCollection<Cell const>> getCellsAttachedToNode(Node::CPtr const & node) const;
    boost::optional<EntityCollection<Cell const>> getCellsAttachedToFace(Face::CPtr const & face) const;
    Cell::CPtr                                    getOtherCell(Face::CPtr const & face, Cell::CPtr const & cell) const;

private:
    NodeConnectivity node_connectivity_;
    FaceConnectivity face_connectivity_;
};
