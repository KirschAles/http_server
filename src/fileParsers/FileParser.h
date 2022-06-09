#ifndef HTTP_SERVER_FILEPARSER_H
#define HTTP_SERVER_FILEPARSER_H
#include <fstream>
#include <iostream>
#include <cstdio>
#include <map>
#include <experimental/filesystem>
#include "../configuration.h"

/**
 * Base class for file parsers
 */
class FileParser {
protected:
    size_t chunkSize;
    virtual size_t calculateSize() const {return 0;}
public:
    FileParser(size_t chunkSize)
            : chunkSize(chunkSize) {}

    /**
    * @return std::string, chunk sized part of the file
    *
    */
    virtual std::string getChunk() {return std::string();}
    /**
     * @return boolean, true if all bytes were already sent forward, false otherwise
     * Checks if all the data from FileParser was sent
     */
    virtual bool isEmpty() const {return true;}
    /**
     * @return size_t number of bytes in data
     * Gets size of the file.
     */
    virtual size_t getSize() const {return 0;}
    /**
     * virtual Destructor, here so the FileParser will inhert it
     */
    virtual ~FileParser() {}
    virtual std::string getType() const {return "";}
};


#endif //HTTP_SERVER_FILEPARSER_H
