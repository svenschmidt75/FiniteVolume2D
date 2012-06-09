/*
 * Name  : ASCIIMeshReaderFaceState
 * Path  : IMeshReaderState
 * Use   : Represents the base state. Read face data.
 * Author: Sven Schmidt
 * Date  : 03/04/2012
 */
#pragma once

#include "IMeshReaderState.h"

class IMeshBuilder;


class ASCIIMeshReaderFaceState : public IMeshReaderState {
public:
    // FROM IMeshReaderState
    bool process(std::vector<std::string> const & tokens, int line);

    bool inNodeMode() const;
    bool inFaceMode() const;
    bool inCellMode() const;
    bool inBoundaryConditionModeMode() const;


    static Ptr create(IMeshBuilder & mesh_builder);

private:
    ASCIIMeshReaderFaceState(IMeshBuilder & mesh_builder);

    // no implicit assignment operator due to reference (mesh_builder_)
    ASCIIMeshReaderFaceState & operator=(ASCIIMeshReaderFaceState const & in);

private:
    IMeshBuilder & mesh_builder_;
};
