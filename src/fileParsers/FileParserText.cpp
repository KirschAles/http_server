#include "FileParserText.h"
FileParserText::FileParserText(const fs::path &filePath, size_t chunkSize)
: FileParserRegular(filePath, chunkSize) {}

std::string FileParserText::getType() const {return mixed::text;}