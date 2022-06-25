#include "FileParserBinary.h"

FileParserBinary::FileParserBinary(const fs::path &filePath, size_t chunkSize)
: FileParserRegular(filePath, chunkSize, std::ios_base::binary | std::ios_base::in) {}