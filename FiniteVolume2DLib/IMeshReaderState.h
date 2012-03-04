/*
 * Name  : IMeshReaderState
 * Path  : 
 * Use   : Represents states the mesh reader is in
 * Author: Sven Schmidt
 * Date  : 03/04/2012
 */
#pragma once

#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>


class IMeshReaderState {
public:
    typedef boost::shared_ptr<IMeshReaderState> Ptr;

public:
    ~IMeshReaderState() {}

    virtual bool process(std::vector<std::string> const & tokens, int line) = 0;

    virtual bool inVertexMode() const = 0;
    virtual bool inFaceMode() const = 0;
    virtual bool inCellMode() const = 0;
};
