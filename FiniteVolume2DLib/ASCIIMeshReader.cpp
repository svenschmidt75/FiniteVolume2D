#include "ASCIIMeshReader.h"

#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>
#include "boost/format.hpp"

#include "Util.h"

namespace FS = boost::filesystem;


ASCIIMeshReader::ASCIIMeshReader(std::string const & mesh_filename, IMeshBuilder & builder)
    : mesh_filename_(mesh_filename), builder_(builder) {}

bool
ASCIIMeshReader::read() const {
    if (!FS::exists(mesh_filename_)) {
        boost::format format = boost::format("ASCIIMeshReader::read: Mesh file %1% not found!\n") % mesh_filename_;
        return Util::error(format.str());
    }

    return true;
}
