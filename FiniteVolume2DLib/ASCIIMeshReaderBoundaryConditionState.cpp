#include "ASCIIMeshReaderBoundaryConditionState.h"

#include "Util.h"
#include "IMeshBuilder.h"
#include "BoundaryConditionCollection.h"

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>


ASCIIMeshReaderBoundaryConditionState::ASCIIMeshReaderBoundaryConditionState(BoundaryConditionCollection & bc) : bc_(bc) {}

namespace {
    BoundaryConditionCollection::Type boundaryConditionType(char c) {
        switch (c) {
            case 'd':
                return BoundaryConditionCollection::DIRICHLET;

            case 'n':
                return BoundaryConditionCollection::NEUMANN;

            default:
                return BoundaryConditionCollection::UNKNOWN;
        }
    }

}

bool
ASCIIMeshReaderBoundaryConditionState::process(std::vector<std::string> const & tokens, int line) {
    if (tokens.size() != 3) {
        boost::format format = boost::format("ASCIIMeshReaderBoundaryConditionState::process: Invalid boundary condition format in line %1%!\n") % line;
        return Util::error(format.str());
    }

    // read face mesh id
    IGeometricEntity::Id_t face_id = boost::lexical_cast<IGeometricEntity::Id_t>(tokens[0]);

    // read type of b.c.
    // d: Dirichlet
    // n: von Neumann
    BoundaryConditionCollection::Type bc_type = boundaryConditionType(tokens[1][0]);
    if (bc_type == BoundaryConditionCollection::UNKNOWN) {
        boost::format format = boost::format("ASCIIMeshReaderBoundaryConditionState::process: Unknown boundary condition in line %1%!\n") % line;
        return Util::error(format.str());
    }

    // extract boundary condition value
    double bc_value = boost::lexical_cast<double>(tokens[2]);

    bc_.add(face_id, bc_type, bc_value);
    return true;
}

bool
ASCIIMeshReaderBoundaryConditionState::inNodeMode() const {
    return false;
}

bool
ASCIIMeshReaderBoundaryConditionState::inFaceMode() const {
    return false;
}

bool
ASCIIMeshReaderBoundaryConditionState::inCellMode() const {
    return false;
}

bool
ASCIIMeshReaderBoundaryConditionState::inBoundaryConditionModeMode() const {
    return true;
}

IMeshReaderState::Ptr
ASCIIMeshReaderBoundaryConditionState::create(BoundaryConditionCollection & bc) {
    return Ptr(new ASCIIMeshReaderBoundaryConditionState(bc));
}
