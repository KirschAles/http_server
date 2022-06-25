#include "FileParser.h"

FileParser::FileParser(size_t chunkSize)
: chunkSize(chunkSize) {}

std::string FileParser::getChunk() {return std::string();}

bool FileParser::isEmpty() const {return true;}

size_t FileParser::getSize() const {return 0;}

std::string FileParser::getType() const {return "";}

size_t FileParser::calculateSize() const {return 0;}
