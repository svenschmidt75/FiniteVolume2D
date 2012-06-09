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
#include "IMeshReaderState.h"
#include "BoundaryConditionCollection.h"

class IMeshBuilder;


#pragma warning(disable:4251)


class DECL_SYMBOLS_2DLIB ASCIIMeshReader : public IMeshReader {
public:
    ASCIIMeshReader(std::string const & mesh_filename, IMeshBuilder & builder);

    // FROM IMeshReader
    bool                                read() const;
    BoundaryConditionCollection const & getBoundaryConditions() const;

private:
    // no implicit assignment operator due to reference (builder_)
    ASCIIMeshReader & operator=(ASCIIMeshReader const & in);

private:
    std::string                         mesh_filename_;
    IMeshBuilder &                      builder_;
    mutable BoundaryConditionCollection bc_;
    mutable IMeshReaderState::Ptr       state_;
};

#pragma warning(default:4251)
