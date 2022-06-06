#ifndef HTTP_SERVER_FILEPARSERDIRECTORY_H
#define HTTP_SERVER_FILEPARSERDIRECTORY_H
#include "FileParserScript.h"

class FileParserDirectory: public FileParserScript {
private:
public:
    FileParserDirectory(const fs::path &filePath, size_t chunkSize)
            : FileParserScript(std::string("ls -w1 ") + filePath.string(), chunkSize) {}
};


#endif //HTTP_SERVER_FILEPARSERDIRECTORY_H
