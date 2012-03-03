/*
 * Name  : IMeshReader
 * Path  : 
 * Use   : Mesh reader interface
 * Author: Sven Schmidt
 * Date  : 03/03/2012
 */
#pragma once

#include "DeclSpec.h"


class DECL_SYMBOLS IMeshReader {
public:
    ~IMeshReader() {}

    virtual bool read() const = 0;
};
