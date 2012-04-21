/*
 * Name  : IMeshReader
 * Path  : 
 * Use   : Mesh reader interface
 * Author: Sven Schmidt
 * Date  : 03/03/2012
 */
#pragma once

#include "DeclSpec.h"

class BoundaryConditionCollection;


class DECL_SYMBOLS_2DLIB IMeshReader {
public:
    virtual ~IMeshReader() {}

    virtual bool                                read() const = 0;
    virtual BoundaryConditionCollection const & getBoundaryConditions() const = 0;
};
