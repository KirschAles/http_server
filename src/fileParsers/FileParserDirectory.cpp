#include "FileParserDirectory.h"

FileParserDirectory::FileParserDirectory(const fs::path &filePath, size_t chunkSize)
: FileParserScript(std::string("ls -w1 ") + filePath.string(), chunkSize) {}