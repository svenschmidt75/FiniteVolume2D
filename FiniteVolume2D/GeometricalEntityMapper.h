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

#include "DeclSpec.h"

#include "ComputationalNode.h"
#include "ComputationalFace.h"
#include "ComputationalCell.h"
#include "FiniteVolume2DLib/Node.h"
#include "FiniteVolume2DLib/Face.h"
#include "FiniteVolume2DLib/Cell.h"

#include <map>


#pragma warning(disable:4251)


class DECL_SYMBOLS_2D GeometricalEntityMapper {
private:
    template<typename GEOMETRICAL_ENTITY, typename COMPUTATIONAL_ENTITY>
    struct Link {
        Link() : gentity_(NULL), centity_(NULL) {}
        explicit Link(typename GEOMETRICAL_ENTITY::CPtr const & gentity, typename COMPUTATIONAL_ENTITY::CPtr const & centity) : gentity_(gentity), centity_(centity) {}

        typename GEOMETRICAL_ENTITY::CPtr   gentity_;
        typename COMPUTATIONAL_ENTITY::CPtr centity_;

    };

public:
    void addNode(Node::CPtr const & node, ComputationalNode::CPtr const & cnode);
    void addFace(Face::CPtr const & face, ComputationalFace::CPtr const & cface);
    void addCell(Cell::CPtr const & cell, ComputationalCell::CPtr const & ccell);

    ComputationalNode::CPtr const & getComputationalNode(Node::CPtr const & node) const;
    ComputationalFace::CPtr const & getComputationalFace(Face::CPtr const & face) const;
    ComputationalCell::CPtr const & getComputationalCell(Cell::CPtr const & cell) const;

private:
    typedef std::map<IGeometricEntity::Id_t, Link<Node, ComputationalNode>> ComputationalNodeMap_t;
    typedef std::map<IGeometricEntity::Id_t, Link<Face, ComputationalFace>> ComputationalFaceMap_t;
    typedef std::map<IGeometricEntity::Id_t, Link<Cell, ComputationalCell>> ComputationalCellMap_t;

private:
    mutable ComputationalNodeMap_t node_map_;
    mutable ComputationalFaceMap_t face_map_;
    mutable ComputationalCellMap_t cell_map_;
};

#pragma warning(default:4251)
