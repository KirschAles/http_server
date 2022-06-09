#ifndef HTTP_SERVER_FILEPARSERTEXT_H
#define HTTP_SERVER_FILEPARSERTEXT_H
#include "FileParserRegular.h"

/**
 * FileParserText
 *
 * behaves in the same exact way as FileParserRegular, just a placeholder to make the classes clearer
 */
class FileParserText: public FileParserRegular {
public:
    FileParserText(const fs::path &filePath, size_t chunkSize)
            : FileParserRegular(filePath, chunkSize) {}
};


#endif //HTTP_SERVER_FILEPARSERTEXT_H
