#include "ASCIIMeshReader.h"

#include "ASCIIMeshReaderBaseState.h"
#include "ASCIIMeshReaderNodeState.h"
#include "ASCIIMeshReaderFaceState.h"
#include "ASCIIMeshReaderCellState.h"
#include "ASCIIMeshReaderBoundaryConditionState.h"
#include "Util.h"
#include "IMeshBuilder.h"

#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>
#include "boost/format.hpp"
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include <iostream>

namespace FS = boost::filesystem;


ASCIIMeshReader::ASCIIMeshReader(std::string const & mesh_filename, IMeshBuilder & builder)
    : mesh_filename_(mesh_filename), builder_(builder) {

    state_.reset();
}

bool
ASCIIMeshReader::read() const {
    if (!FS::exists(mesh_filename_)) {
        FS::path p = FS::initial_path();
        boost::format format = boost::format("ASCIIMeshReader::read: Mesh file %1% not found!\nCurrent path: %2%\n") % mesh_filename_ % p;
        return Util::error(format.str());
    }

    /* 
     * State 0: expect to read vertex keyword
     * State 1: in vertex mode, expect to read face keyword
     * State 2: in face mode, expect to read cell keyword
     * State 3: in cell mode
     * State 4: in boundary condition mode
     */

    boost::iostreams::stream<boost::iostreams::file_source> file(mesh_filename_);
    std::string line;
    boost::char_separator<char> sep(" ");

    int line_number = 0;

    state_ = ASCIIMeshReaderBaseState::create(builder_);

    while (std::getline(file, line)) {
        typedef boost::tokenizer<boost::char_separator<char>> TokenizerType;

        line_number++;

        boost::trim(line);
        TokenizerType tokens(line, sep);

        std::vector<std::string> token_arr(tokens.begin(), tokens.end());

        if (!token_arr.empty()) {
            std::string t = *(tokens.begin());
            boost::algorithm::to_lower(t);

            // Comment?
            if (t[0] == '#')
                continue;


            if (t == "vertices") {
                if (state_->inNodeMode() || state_->inFaceMode() || state_->inCellMode()) {
                    boost::format format = boost::format("ASCIIMeshReader::read: Keyword 'vertices' unexpected in line %1%!\n") % line_number;
                    return Util::error(format.str());
                }
                state_ = ASCIIMeshReaderNodeState::create(builder_);
                continue;
            }
            else if (t == "faces") {
                if (!state_->inNodeMode()) {
                    boost::format format = boost::format("ASCIIMeshReader::read: Keyword 'faces' unexpected in line %1%!\n") % line_number;
                    return Util::error(format.str());
                }
                state_ = ASCIIMeshReaderFaceState::create(builder_);
                continue;
            }
            else if (t == "cells") {
                if (!state_->inFaceMode()) {
                    boost::format format = boost::format("ASCIIMeshReader::read: Keyword 'cells' unexpected in line %1%!\n") % line_number;
                    return Util::error(format.str());
                }
                state_ = ASCIIMeshReaderCellState::create(builder_);
                continue;
            }
            else if (t == "boundaryconditions") {
                if (!state_->inCellMode()) {
                    boost::format format = boost::format("ASCIIMeshReader::read: Keyword 'boundaryconditions' unexpected in line %1%!\n") % line_number;
                    return Util::error(format.str());
                }
                state_ = ASCIIMeshReaderBoundaryConditionState::create(bc_);
                continue;
            }

            try {
                if (!state_->process(token_arr, line_number))
                    return false;
            }
            catch (boost::exception const &) {
                boost::format format = boost::format("ASCIIMeshReader::read: Input error in line %1%!\n") % line_number;
                return Util::error(format.str());
            }
        }
    }

    builder_.outputReport(std::cout);

    return true;
}

BoundaryConditionCollection const &
ASCIIMeshReader::getBoundaryConditions() const {
    return bc_;
}
