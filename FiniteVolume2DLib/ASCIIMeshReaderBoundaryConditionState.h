/*
 * Name  : ASCIIMeshReaderBoundaryConditionState
 * Path  : IMeshReaderState
 * Use   : Reads boundary condition from mesh file.
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "IMeshReaderState.h"

class IMeshBuilder;


class ASCIIMeshReaderBoundaryConditionState : public IMeshReaderState {
public:
    // FROM IMeshReaderState
    bool process(std::vector<std::string> const & tokens, int line);

    bool inNodeMode() const;
    bool inFaceMode() const;
    bool inCellMode() const;
    bool inBoundaryConditionModeMode() const;


    static Ptr create(IMeshBuilder & mesh_builder);

private:
    ASCIIMeshReaderBoundaryConditionState(IMeshBuilder & mesh_builder);

private:
    IMeshBuilder & mesh_builder_;
};
