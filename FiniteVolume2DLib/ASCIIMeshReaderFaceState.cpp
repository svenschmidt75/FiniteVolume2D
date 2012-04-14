#include "ASCIIMeshReaderFaceState.h"

#include "Util.h"
#include "IMeshBuilder.h"

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>


ASCIIMeshReaderFaceState::ASCIIMeshReaderFaceState(IMeshBuilder & mesh_builder) : mesh_builder_(mesh_builder) {}

bool
ASCIIMeshReaderFaceState::process(std::vector<std::string> const & tokens, int line) {
    if (tokens.size() < 4) {
        boost::format format = boost::format("ASCIIMeshReaderFaceState::process: Invalid face format in line %1%!\n") % line;
        return Util::error(format.str());
    }

    // Lambda function
    std::vector<IGeometricEntity::Id_t> ids;
    std::for_each(tokens.begin() + 2, tokens.end(), [&ids](std::string const & t) {
        IGeometricEntity::Id_t id = boost::lexical_cast<IGeometricEntity::Id_t>(t);
        ids.push_back(id);
    });

    IGeometricEntity::Id_t face_id = boost::lexical_cast<IGeometricEntity::Id_t>(tokens[0]);
    bool on_boundary = boost::lexical_cast<bool>(tokens[1]);
    mesh_builder_.buildFace(face_id, on_boundary ? IGeometricEntity::BOUNDARY : IGeometricEntity::INTERIOR, ids);
    return true;
}

bool
ASCIIMeshReaderFaceState::inNodeMode() const {
    return false;
}

bool
ASCIIMeshReaderFaceState::inFaceMode() const {
    return true;
}

bool
ASCIIMeshReaderFaceState::inCellMode() const {
    return false;
}

bool
ASCIIMeshReaderFaceState::inBoundaryConditionModeMode() const {
    return false;
}

IMeshReaderState::Ptr
ASCIIMeshReaderFaceState::create(IMeshBuilder & mesh_builder) {
    return Ptr(new ASCIIMeshReaderFaceState(mesh_builder));
}
