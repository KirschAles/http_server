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

namespace fs = std::experimental::filesystem;

class ContentGenerator {
private:
    const Configuration &configuration;
    FileParser *fileParser = nullptr;
    std::map<std::string, std::string> headers;

    // directory must be directory, should be checked by the configuration
    static bool isSubdirectory(const fs::path &file, const fs::path &directory);
    // TO DO: replace this with endings that should be loaded from configuration file
    //        this is just a temporary solution
    FileParser *createFileParserOfRegular(const fs::path &file);
    FileParser *createFileParser(const fs::path &file);

    void buildHeaders();
public:
    ContentGenerator(const std::string fileName, const Configuration &configuration);
    ContentGenerator(ContentGenerator &&contentGenerator)
    : configuration(contentGenerator.configuration),
      fileParser(contentGenerator.fileParser),
      headers(std::move(contentGenerator.headers)) {
        // set to NULL so the parser isn't destroyed when original deconstructs
        contentGenerator.fileParser = nullptr;
    }
    const std::map<std::string, std::string> &getHeaders() const;
    bool isEmpty() { return fileParser->isEmpty();}
    std::string getChunk() { return std::move(fileParser->getChunk());}
    ~ContentGenerator();
};


#endif //HTTP_SERVER_CONTENTGENERATOR_H
