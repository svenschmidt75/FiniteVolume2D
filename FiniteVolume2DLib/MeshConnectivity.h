/*
 * Name  : MeshConnectivity
 * Path  : 
 * Use   : Contains mesh connectivity, like
 *         - neighbor vertices of a vertex
 *         - faces attached to a vertex
 *         - cells attached to a vertex
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
#include "Vertex.h"
#include "Face.h"
#include "Cell.h"
#include "EntityCollection.hpp"
#include "VertexConnectivity.h"
#include "FaceConnectivity.h"

#include <map>
#include <boost/optional.hpp>


class MeshConnectivity : public IMeshConnectivity {
public:
    // FROM IMeshConnectivity
    void                                      insert(Face::Ptr const & face);
    void                                      insert(Cell::Ptr const & cell);
    boost::optional<EntityCollection<Vertex>> getVertexNeighbors(Vertex::Ptr const & vertex) const;
    boost::optional<EntityCollection<Face>>   getFacesAttachedToVertex(Vertex::Ptr const & vertex) const;
    boost::optional<EntityCollection<Cell>>   getCellsAttachedToVertex(Vertex::Ptr const & vertex) const;
    boost::optional<EntityCollection<Cell>>   getCellsAttachedToFace(Face::Ptr const & face) const;
    Cell::Ptr                                 getOtherCell(Face::Ptr const & face, Cell::Ptr const & cell) const;

private:
    VertexConnectivity vertex_connectivity_;
    FaceConnectivity   face_connectivity_;
};
