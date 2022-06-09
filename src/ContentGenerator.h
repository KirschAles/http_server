#ifndef HTTP_SERVER_CONTENTGENERATOR_H
#define HTTP_SERVER_CONTENTGENERATOR_H
#include <fstream>
#include <iostream>
#include <cstdio>
#include <map>
#include <experimental/filesystem>
#include "configuration.h"
#include "fileParsers/FileParser.h"
#include "fileParsers/FileParserRegular.h"
#include "fileParsers/FileParserScript.h"
#include "fileParsers/FileParserDirectory.h"
#include "fileParsers/FileParserBinary.h"
#include "fileParsers/FileParserText.h"
#include "exceptions/BadRequest.h"
#include "exceptions/InternalServerError.h"


namespace fs = std::experimental::filesystem;

/**
 * ContentGenerator
 *
 * It's main use is to create and manage FileParsers and
 * resend used data from that forward.
 *
 */
class ContentGenerator {
private:
    const Configuration &configuration;
    FileParser *fileParser = nullptr;
    std::map<std::string, std::string> headers;
    fs::path fileName;

    // directory must be directory, should be checked by the configuration
    static bool isSubdirectory(const fs::path &file, const fs::path &directory);

    FileParser *createFileParserOfRegular(const fs::path &file);
    FileParser *createFileParser(const fs::path &file);
    bool isEqual(const fs::path &file1, const fs::path &file2) const;
    fs::path removeDots(const fs::path &file) const;
    void buildHeaders();
public:
    ContentGenerator(const std::string fileName, const Configuration &configuration);
    ContentGenerator(ContentGenerator &&contentGenerator)
    : configuration(contentGenerator.configuration),
      fileParser(contentGenerator.fileParser),
      headers(std::move(contentGenerator.headers)),
      fileName(contentGenerator.fileName) {
        // set to NULL so the parser isn't destroyed when original deconstructs
        contentGenerator.fileParser = nullptr;
    }
    const std::map<std::string, std::string> &getHeaders() const;
    bool isEmpty() { return fileParser->isEmpty();}
    std::string getChunk() { return std::move(fileParser->getChunk());}
    ~ContentGenerator();
};


#endif //HTTP_SERVER_CONTENTGENERATOR_H
