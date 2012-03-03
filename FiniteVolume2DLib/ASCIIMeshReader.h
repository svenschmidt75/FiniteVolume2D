/*
 * Name  : ASCIIMeshReader
 * Path  : IMeshReader
 * Use   : Reads a mesh file in ascii format
 * Author: Sven Schmidt
 * Date  : 03/03/2012
 */
#pragma once

#include <string>

#include "DeclSpec.h"
#include "IMeshReader.h"

class IMeshBuilder;


class DECL_SYMBOLS ASCIIMeshReader : public IMeshReader {
public:
    ASCIIMeshReader(std::string const & mesh_filename, IMeshBuilder & builder);

    // FROM IMeshReader
    bool read() const;

private:
    std::string mesh_filename_;
    IMeshBuilder & builder_;
};
