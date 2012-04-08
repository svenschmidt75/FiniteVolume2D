/*
 * Name  : ComputationalNode
 * Path  : INode
 * Use   : Computational node
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "FiniteVolume2DLib/INode.h"

#include "ComputationalMoleculeManager.h"
#include "FiniteVolume2DLib/IGeometricEntity.h"
#include "FiniteVolume2DLib/Vector.h"
#include "FiniteVolume2DLib/Node.h"


class ComputationalNode : public INode {
public:
    explicit ComputationalNode(Node::Ptr const & geometric_node);

    // FROM IGeometricEntity
    IGeometricEntity::Id_t id() const;
    IGeometricEntity::Id_t meshId() const;

    // FROM INode
    bool   onBoundary() const;
    double x() const;
    double y() const;
    Vector vector() const;

private:
    // the geometric partner node
    Node::Ptr geometric_node_;

    /* A computational node may have a set of molecules,
     * but does not have to.
     */
    ComputationalMoleculeManager::Ptr molecule_;
};
