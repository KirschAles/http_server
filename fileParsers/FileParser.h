#ifndef HTTP_SERVER_FILEPARSER_H
#define HTTP_SERVER_FILEPARSER_H
#include <fstream>
#include <iostream>
#include <cstdio>
#include <map>
#include <experimental/filesystem>
#include "../configuration.h"

class FileParser {
protected:
    size_t chunkSize;
    virtual size_t calculateSize() const {return 0;}
public:
    FileParser(size_t chunkSize)
            : chunkSize(chunkSize) {}
    virtual std::string getChunk() {return std::string();}
    virtual bool isEmpty() const {return true;}
    virtual size_t getSize() const {return 0;}
    virtual ~FileParser() {}
};


#endif //HTTP_SERVER_FILEPARSER_H
