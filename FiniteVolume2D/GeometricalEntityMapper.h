/*
 * Name  : GeometricalEntityMapper
 * Path  : 
 * Use   : Links the computational entities to the geometrical ones.
 *         The is done because the mesh connectivity is purely a
 *         geometrical concept. Also, this allows to link the same
 *         geometrical entity to multiple computational ones, which
 *         is needed for multigrid methods.
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "ComputationalNode.h"
#include "ComputationalFace.h"
#include "ComputationalCell.h"
#include "FiniteVolume2DLib/Node.h"
#include "FiniteVolume2DLib/Face.h"
#include "FiniteVolume2DLib/Cell.h"

#include <map>


class GeometricalEntityMapper {
private:
    template<typename GEOMETRICAL_ENTITY, typename COMPUTATIONAL_ENTITY>
    struct Link {
        Link() : gentity_(NULL), centity_(NULL) {}
        explicit Link(typename GEOMETRICAL_ENTITY::Ptr const & gentity, typename COMPUTATIONAL_ENTITY::Ptr const & centity) : gentity_(gentity), centity_(centity) {}

        typename GEOMETRICAL_ENTITY::Ptr   gentity_;
        typename COMPUTATIONAL_ENTITY::Ptr centity_;

    };

public:
    void addNode(Node::Ptr const & node, ComputationalNode::Ptr const & cnode);
    void addFace(Face::Ptr const & face, ComputationalFace::Ptr const & cface);
    void addCell(Cell::Ptr const & cell, ComputationalCell::Ptr const & ccell);

    ComputationalNode::Ptr const & getComputationalNode(Node::Ptr const & node) const;
    ComputationalFace::Ptr const & getComputationalFace(Face::Ptr const & face) const;
    ComputationalCell::Ptr const & getComputationalCell(Cell::Ptr const & cell) const;

private:
    typedef std::map<IGeometricEntity::Id_t, Link<Node, ComputationalNode>> ComputationalNodeMap_t;
    typedef std::map<IGeometricEntity::Id_t, Link<Face, ComputationalFace>> ComputationalFaceMap_t;
    typedef std::map<IGeometricEntity::Id_t, Link<Cell, ComputationalCell>> ComputationalCellMap_t;

private:
    mutable ComputationalNodeMap_t node_map_;
    mutable ComputationalFaceMap_t face_map_;
    mutable ComputationalCellMap_t cell_map_;
};
