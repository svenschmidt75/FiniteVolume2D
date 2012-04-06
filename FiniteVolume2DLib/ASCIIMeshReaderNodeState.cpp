#include "ASCIIMeshReaderNodeState.h"

#include "Util.h"
#include "IMeshBuilder.h"

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>


ASCIIMeshReaderNodeState::ASCIIMeshReaderNodeState(IMeshBuilder & mesh_builder) : mesh_builder_(mesh_builder) {}

bool
ASCIIMeshReaderNodeState::process(std::vector<std::string> const & tokens, int line) {
    if (tokens.size() != 4) {
        boost::format format = boost::format("ASCIIMeshReaderVertexState::process: Invalid vertex format in line %1%!\n") % line;
        return Util::error(format.str());
    }

    IGeometricEntity::Id_t id = boost::lexical_cast<IGeometricEntity::Id_t>(tokens[0]);
    bool on_boundary = boost::lexical_cast<bool>(tokens[1]);
    double x = boost::lexical_cast<double>(tokens[2]);
    double y = boost::lexical_cast<double>(tokens[3]);
    mesh_builder_.buildNode(id, on_boundary, x, y);
    return true;
}

bool
ASCIIMeshReaderNodeState::inNodeMode() const {
    return true;
}

bool
ASCIIMeshReaderNodeState::inFaceMode() const {
    return false;
}

bool
ASCIIMeshReaderNodeState::inCellMode() const {
    return false;
}

IMeshReaderState::Ptr
ASCIIMeshReaderNodeState::create(IMeshBuilder & mesh_builder) {
    return Ptr(new ASCIIMeshReaderNodeState(mesh_builder));
}
