#ifndef HTTP_SERVER_CONTENTGENERATOR_H
#define HTTP_SERVER_CONTENTGENERATOR_H
#include <fstream>
#include <iostream>
#include <cstdio>
#include <map>
#include <experimental/filesystem>
#include <memory>
#include "configuration.h"
#include "fileParsers/FileParser.h"
#include "fileParsers/FileParserRegular.h"
#include "fileParsers/FileParserScript.h"
#include "fileParsers/FileParserDirectory.h"
#include "fileParsers/FileParserBinary.h"
#include "fileParsers/FileParserText.h"
#include "exceptions/BadRequest.h"
#include "exceptions/InternalServerError.h"
#include "fileParsers/FileParserImage.h"


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
    std::unique_ptr<FileParser> fileParser;
    std::map<std::string, std::string> headers;
    fs::path fileName;

    // directory must be directory, should be checked by the configuration
    static bool isSubdirectory(const fs::path &file, const fs::path &directory);

    std::unique_ptr<FileParser> createFileParserOfRegular(const fs::path &file);
    std::unique_ptr<FileParser> createFileParser(const fs::path &file);
    bool isEqual(const fs::path &file1, const fs::path &file2) const;
    fs::path removeDots(const fs::path &file) const;
    void buildHeaders();
public:
    ContentGenerator(const std::string fileName, const Configuration &configuration);
    ContentGenerator(ContentGenerator &&contentGenerator);
    const std::map<std::string, std::string> &getHeaders() const;
    bool isEmpty();
    std::string getChunk();
    ~ContentGenerator();
};


#endif //HTTP_SERVER_CONTENTGENERATOR_H
