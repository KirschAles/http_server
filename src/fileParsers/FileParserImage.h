#ifndef HTTP_SERVER_FILEPARSERIMAGE_H
#define HTTP_SERVER_FILEPARSERIMAGE_H
#include "FileParserBinary.h"
class FileParserImage: public FileParserBinary {
public:
    FileParserImage(const fs::path &filePath, size_t chunkSize);
    std::string getType() const override;
};

#endif //HTTP_SERVER_FILEPARSERIMAGE_H
