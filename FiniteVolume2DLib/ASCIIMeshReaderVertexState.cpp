#include "ASCIIMeshReaderVertexState.h"

#include "Util.h"
#include "IMeshBuilder.h"

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>


ASCIIMeshReaderVertexState::ASCIIMeshReaderVertexState(IMeshBuilder & mesh_builder) : mesh_builder_(mesh_builder) {}

bool
ASCIIMeshReaderVertexState::process(std::vector<std::string> const & tokens, int line) {
    if (tokens.size() != 4) {
        boost::format format = boost::format("ASCIIMeshReaderVertexState::process: Invalid vertex format in line %1%!\n") % line;
        return Util::error(format.str());
    }

    IGeometricEntity::Id_t id = boost::lexical_cast<IGeometricEntity::Id_t>(tokens[0]);
    bool on_boundary = boost::lexical_cast<bool>(tokens[1]);
    double x = boost::lexical_cast<double>(tokens[2]);
    double y = boost::lexical_cast<double>(tokens[3]);
    mesh_builder_.buildVertex(id, on_boundary, x, y);
    return true;
}

bool
ASCIIMeshReaderVertexState::inVertexMode() const {
    return true;
}

bool
ASCIIMeshReaderVertexState::inFaceMode() const {
    return false;
}

bool
ASCIIMeshReaderVertexState::inCellMode() const {
    return false;
}

IMeshReaderState::Ptr
ASCIIMeshReaderVertexState::create(IMeshBuilder & mesh_builder) {
    return Ptr(new ASCIIMeshReaderVertexState(mesh_builder));
}
