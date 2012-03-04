#include "ASCIIMeshReaderBaseState.h"

#include "Util.h"

#include <boost/format.hpp>


ASCIIMeshReaderBaseState::ASCIIMeshReaderBaseState(IMeshBuilder & mesh_builder) : mesh_builder_(mesh_builder) {}

bool
ASCIIMeshReaderBaseState::process(std::vector<std::string> const & tokens, int line) {
    boost::format format = boost::format("ASCIIMeshReaderBaseState::process: Invalid input in line %1%!\n") % line;
    return Util::error(format.str());
}

bool
ASCIIMeshReaderBaseState::inVertexMode() const {
    return false;
}

bool
ASCIIMeshReaderBaseState::inFaceMode() const {
    return false;
}

bool
ASCIIMeshReaderBaseState::inCellMode() const {
    return false;
}

IMeshReaderState::Ptr
ASCIIMeshReaderBaseState::create(IMeshBuilder & mesh_builder) {
    return Ptr(new ASCIIMeshReaderBaseState(mesh_builder));
}
