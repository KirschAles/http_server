#ifndef HTTP_SERVER_FILEPARSERBINARY_H
#define HTTP_SERVER_FILEPARSERBINARY_H
#include "FileParserRegular.h"


/**
 * FileParserBinary same as FileParserRegular, but with binary flag added
 */
class FileParserBinary: public FileParserRegular {
public:
    FileParserBinary(const fs::path &filePath, size_t chunkSize);
};


#endif //HTTP_SERVER_FILEPARSERBINARY_H
