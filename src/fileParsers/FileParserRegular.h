#ifndef HTTP_SERVER_FILEPARSERREGULAR_H
#define HTTP_SERVER_FILEPARSERREGULAR_H
#include "FileParser.h"

/**
 * FileParserRegular
 *
 * Deals with normal files with data.
 */
class FileParserRegular: public FileParser {
protected:
    std::ifstream file;
    const fs::path &path;
    size_t size;
    size_t calculateSize() const override;
public:
    FileParserRegular(const fs::path &filePath,
                      size_t chunkSize,
                      std::ios_base::openmode mode = std::ios_base::in);
    std::string getChunk() override;
    bool isEmpty() const override;
    size_t getSize() const override;
    ~FileParserRegular() override;
};


#endif //HTTP_SERVER_FILEPARSERREGULAR_H
