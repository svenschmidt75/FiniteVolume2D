/*
 * Name  : Mesh
 * Path  : IMesh
 * Use   : Mesh class
 * Author: Sven Schmidt
 * Date  : 03/03/2012
 */
#pragma once

#include "IMesh.h"

#include <boost/shared_ptr.hpp>


class Mesh : public IMesh {
public:
    typedef boost::shared_ptr<Mesh> Ptr;
};
