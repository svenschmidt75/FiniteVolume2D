/*
 * Name  : MeshConnectivity
 * Path  : 
 * Use   : Contains mesh connectivity, like
 *         - neighbor vertices of a vertex etc.
 * Author: Sven Schmidt
 * Date  : 03/10/2012
 */
#pragma once

#include "Vertex.h"
#include "Face.h"
#include "EntityCollection.hpp"

#include <map>
#include <boost/optional.hpp>


class MeshConnectivity {
public:
//    Cell::Ptr                       getOtherCell(Face::Ptr & face, Cell::Ptr & cell) const;
    void                                      insert(Face::Ptr const & face);
    boost::optional<EntityCollection<Vertex>> getVertexNeighbors(Vertex::Ptr & vertex);
//    EntityCollection<Face::Ptr> &   getFaces(Cell::Ptr & cell);

private:
    typedef std::map<IGeometricEntity::Id_t, EntityCollection<Vertex>> VertexNeighbor_t;
    VertexNeighbor_t vertex_neighbors_;
};
