/*
 * Name  : ASCIIMeshReader
 * Path  : IMeshReader
 * Use   : Reads a mesh file in ascii format
 * Author: Sven Schmidt
 * Date  : 03/03/2012
 */
#pragma once

#include <string>

#include "IMeshReader.h"

class MeshBuilder;


class ASCIIMeshReader : public IMeshReader {
public:
    ASCIIMeshReader(std::string const & mesh_filename, MeshBuilder & builder);

    // FROM IMeshReader
    bool read() const;

private:
    std::string mesh_filename_;
    MeshBuilder & builder_;
};
