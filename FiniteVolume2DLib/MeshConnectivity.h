/*
 * Name  : MeshConnectivity
 * Path  : 
 * Use   : Contains mesh connectivity, like
 *         - neighbor vertices of a vertex
 *         - faces attached to a vertex
 *         - cells attached to a vertex
 *         - cells attached to a face
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
    Cell::Ptr                                 getOtherCell(Face::Ptr & face, Cell::Ptr & cell) const;
    void                                      insert(Face::Ptr const & face);
    void                                      insert(Cell::Ptr const & cell);
    boost::optional<EntityCollection<Vertex>> getVertexNeighbors(Vertex::Ptr const & vertex) const;

private:
    VertexConnectivity vertex_connectivity_;
    FaceConnectivity face_connectivity_;
};
