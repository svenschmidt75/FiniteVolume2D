#include "Util.h"

#include <iostream>


bool
Util::error(std::string const & msg) {
    std::cerr << msg << std::endl;
    return false;
}
