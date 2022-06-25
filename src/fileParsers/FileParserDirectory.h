#ifndef HTTP_SERVER_FILEPARSERDIRECTORY_H
#define HTTP_SERVER_FILEPARSERDIRECTORY_H
#include "FileParserScript.h"


/**
 * FileParser to manage directory, it is just FileParserScript with the use of ls
 */
class FileParserDirectory: public FileParserScript {
private:
public:
    FileParserDirectory(const fs::path &filePath, size_t chunkSize);
};


#endif //HTTP_SERVER_FILEPARSERDIRECTORY_H
