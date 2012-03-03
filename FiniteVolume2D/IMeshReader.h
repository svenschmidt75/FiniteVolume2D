/*
 * Name  : IMeshReader
 * Path  : 
 * Use   : Mesh reader interface
 * Author: Sven Schmidt
 * Date  : 03/03/2012
 */
#pragma once


class IMeshReader {
public:
    ~IMeshReader() {}

    virtual bool read() const = 0;
};
