#include "ASCIIMeshReaderCellState.h"
#include "ASCIIMeshReaderFaceState.h"

#include "Util.h"
#include "IMeshBuilder.h"

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>


ASCIIMeshReaderCellState::ASCIIMeshReaderCellState(IMeshBuilder & mesh_builder) : mesh_builder_(mesh_builder) {}

bool
ASCIIMeshReaderCellState::process(std::vector<std::string> const & tokens, int line) {
    if (tokens.size() < 4) {
        boost::format format = boost::format("ASCIIMeshReaderCellState::process: Invalid cell format in line %1%!\n") % line;
        return Util::error(format.str());
    }

    // Lambda function
    std::vector<IGeometricEntity::Id_t> ids;
    std::for_each(tokens.begin() + 1, tokens.end(), [&ids](std::string const & t) {
        IGeometricEntity::Id_t id = boost::lexical_cast<IGeometricEntity::Id_t>(t);
        ids.push_back(id);
    });

    IGeometricEntity::Id_t cell_id = boost::lexical_cast<IGeometricEntity::Id_t>(tokens[0]);
    mesh_builder_.buildCell(cell_id, ids);
    return true;
}

bool
ASCIIMeshReaderCellState::inNodeMode() const {
    return false;
}

bool
ASCIIMeshReaderCellState::inFaceMode() const {
    return false;
}

bool
ASCIIMeshReaderCellState::inCellMode() const {
    return true;
}

IMeshReaderState::Ptr
ASCIIMeshReaderCellState::create(IMeshBuilder & mesh_builder) {
    return Ptr(new ASCIIMeshReaderCellState(mesh_builder));
}
