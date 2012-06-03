/*
 * Name  : MeshChecker
 * Path  : 
 * Use   : Consistency checks for a mesh
 * Author: Sven Schmidt
 * Date  : 06/03/2012
 */
#pragma once

#include "DeclSpec.h"

#include <memory>


class Mesh;
class BoundaryConditionCollection;


class DECL_SYMBOLS_2DLIB MeshChecker {
public:
    static bool checkMesh(std::shared_ptr<Mesh const> const & mesh, BoundaryConditionCollection const & bc);
};
