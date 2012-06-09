/*
 * Name  : ASCIIMeshReaderBoundaryConditionState
 * Path  : IMeshReaderState
 * Use   : Reads boundary condition from mesh file.
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "IMeshReaderState.h"
#include "BoundaryConditionCollection.h"


class ASCIIMeshReaderBoundaryConditionState : public IMeshReaderState {
public:
    // FROM IMeshReaderState
    bool process(std::vector<std::string> const & tokens, int line);

    bool inNodeMode() const;
    bool inFaceMode() const;
    bool inCellMode() const;
    bool inBoundaryConditionModeMode() const;


    static Ptr create(BoundaryConditionCollection & bc);

private:
    ASCIIMeshReaderBoundaryConditionState(BoundaryConditionCollection & bc);

    // no implicit assignment operator due to reference (bc_)
    ASCIIMeshReaderBoundaryConditionState & operator=(ASCIIMeshReaderBoundaryConditionState const & in);

private:
    BoundaryConditionCollection & bc_;
};
