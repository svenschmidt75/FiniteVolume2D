/*
 * Name  : VertexConnectivity
 * Path  : 
 * Use   : Vertex connectivity
 * Author: Sven Schmidt
 * Date  : 03/17/2012
 */
#pragma once

#include "Face.h"
#include "Cell.h"

#include <map>

#include <boost/optional.hpp>

class Vertex;


class VertexConnectivity {
public:
    void                                      insert(Face::Ptr const & face);
    void                                      insert(Cell::Ptr const & cell);
    boost::optional<EntityCollection<Vertex>> getVertexNeighbors(Vertex::Ptr & vertex) const;

private:
    // the neighboring vertices
    typedef std::map<IGeometricEntity::Id_t, EntityCollection<Vertex>> VertexNeighbor_t;
    VertexNeighbor_t vertex_neighbors_;

    // attached faces
    typedef std::map<IGeometricEntity::Id_t, EntityCollection<Face>> VertexFace_t;
    VertexFace_t vertex_faces_;

    // attached cells
    typedef std::map<IGeometricEntity::Id_t, EntityCollection<Cell>> VertexCell_t;
    VertexCell_t vertex_cells_;
};
