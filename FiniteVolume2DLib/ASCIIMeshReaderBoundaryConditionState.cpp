#include "ASCIIMeshReaderBoundaryConditionState.h"

#include "Util.h"
#include "IMeshBuilder.h"
#include "BoundaryCondition.h"

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>


ASCIIMeshReaderBoundaryConditionState::ASCIIMeshReaderBoundaryConditionState(IMeshBuilder & mesh_builder) : mesh_builder_(mesh_builder) {}

namespace {
    BoundaryCondition::Type boundaryConditionType(char c) {
        switch (c) {
            case 'd':
                return BoundaryCondition::DIRICHLET;

            case 'n':
                return BoundaryCondition::NEUMANN;

            default:
                return BoundaryCondition::UNKNOWN;
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
    BoundaryCondition::Type bc_type = boundaryConditionType(tokens[1][0]);
    if (bc_type == BoundaryCondition::UNKNOWN) {
        boost::format format = boost::format("ASCIIMeshReaderBoundaryConditionState::process: Unknown boundary condition in line %1%!\n") % line;
        return Util::error(format.str());
    }

    // extract boundary condition value
    double bc_value = boost::lexical_cast<double>(tokens[2]);

    mesh_builder_.buildBoundaryCondition(face_id, bc_type, bc_value);
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
ASCIIMeshReaderBoundaryConditionState::create(IMeshBuilder & mesh_builder) {
    return Ptr(new ASCIIMeshReaderBoundaryConditionState(mesh_builder));
}
