/*
 * Name  : ComputationalNode
 * Path  : INode
 * Use   : Computational node
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "DeclSpec.h"

#include "FiniteVolume2DLib/INode.h"
#include "ComputationalMolecule.h"

#include "FiniteVolume2DLib/IGeometricEntity.h"
#include "FiniteVolume2DLib/Vector.h"
#include "FiniteVolume2DLib/Node.h"

#include <memory>
#include <string>
#include <map>


#pragma warning(disable:4251)
#pragma warning(disable:4275)


class DECL_SYMBOLS_2D ComputationalNode : public INode {
public:
    typedef std::shared_ptr<ComputationalNode>       Ptr;
    typedef std::shared_ptr<ComputationalNode const> CPtr;

public:
    explicit ComputationalNode(Node::CPtr const & geometric_node);

    // FROM IGeometricEntity
    IGeometricEntity::Id_t id() const;
    IGeometricEntity::Id_t meshId() const;

    // FROM INode
    IGeometricEntity::Entity_t getEntityType() const;
    Vertex                     location() const;

    Node::CPtr const &         geometricEntity() const;

    ComputationalMolecule &    getComputationalMolecule(std::string const & var_name);
    void                       addComputationalMolecule(std::string const & var_name);

private:
    typedef std::map<std::string, ComputationalMolecule> ComputationalMoleculeManager_t;

private:
    // the geometric partner node
    Node::CPtr                     geometric_node_;

    /* A computational node may have a set of molecules,
     * but does not have to.
     */
    ComputationalMoleculeManager_t cm_;
};

#pragma warning(default:4275)
#pragma warning(default:4251)
