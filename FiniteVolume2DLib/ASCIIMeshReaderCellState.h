/*
 * Name  : ASCIIMeshReaderCellState
 * Path  : IMeshReaderState
 * Use   : Represents the base state. Read cell data.
 * Author: Sven Schmidt
 * Date  : 03/04/2012
 */
#pragma once

#include "IMeshReaderState.h"

class IMeshBuilder;


class ASCIIMeshReaderCellState : public IMeshReaderState {
public:
    // FROM IMeshReaderState
    bool process(std::vector<std::string> const & tokens, int line);

    bool inVertexMode() const;
    bool inFaceMode() const;
    bool inCellMode() const;


    static Ptr create(IMeshBuilder & mesh_builder);

private:
    ASCIIMeshReaderCellState(IMeshBuilder & mesh_builder);

private:
    IMeshBuilder & mesh_builder_;
};
