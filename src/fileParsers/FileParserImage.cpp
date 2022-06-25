#include "FileParserImage.h"

FileParserImage::FileParserImage(const fs::path &filePath, size_t chunkSize)
: FileParserBinary(filePath, chunkSize) {}

std::string FileParserImage::getType() const {return mixed::image;}

