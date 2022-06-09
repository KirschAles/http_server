#ifndef HTTP_SERVER_FILEPARSERSCRIPT_H
#define HTTP_SERVER_FILEPARSERSCRIPT_H
#include "FileParser.h"
#include "sys/stat.h"
#include <cstdio>

/**
 * FileParser to manage the output of a script
 */
class FileParserScript: public FileParser {
protected:
    // it will just output the contents of the directory,
    // so it shouldn't be big enough to cause trouble
    FILE *output;
    size_t size;
    std::string currentChunk;
    size_t calculateSize() const override;
    virtual void updateChunk();
    std::string newChunk();
public:
    // the fs::path &path is expected to be already checked for validity
    FileParserScript(const std::string &command, size_t chunkSize);
    // will bug if the script outputs '/0' to console
    std::string getChunk() override;
    bool isEmpty() const override;
    size_t getSize() const override;
    ~FileParserScript() override;
};


#endif //HTTP_SERVER_FILEPARSERSCRIPT_H
