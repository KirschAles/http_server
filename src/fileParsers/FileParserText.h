#ifndef HTTP_SERVER_FILEPARSERTEXT_H
#define HTTP_SERVER_FILEPARSERTEXT_H
#include "FileParserRegular.h"

class FileParserText: public FileParserRegular {
public:
    FileParserText(const fs::path &filePath, size_t chunkSize)
            : FileParserRegular(filePath, chunkSize) {}
};


#endif //HTTP_SERVER_FILEPARSERTEXT_H
