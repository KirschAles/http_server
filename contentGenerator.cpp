#include "contentGenerator.h"
#ifndef HTTP_SERVER_CONTENTGENERATOR_H
#define HTTP_SERVER_CONTENTGENERATOR_H
#include <fstream>
#include <iostream>
#include <cstdio>
#include <map>
#include <experimental/filesystem>
#include "configuration.h"
#include "codes.h"
namespace fs = std::experimental::filesystem;

    // directory must be directory, should be checked by the configuration
    static bool contentGenerator::isSubdirectory(const fs::path &file, const fs::path &directory) {
        if (fs::status(directory).type() != fs::file_type::directory) {
            // this should be checked by config but better to be safe
            throw std::runtime_error("Root is not Directory");
        }
        auto fileIter = file.begin();
        auto dirIter = directory.begin();
        for (;dirIter != directory.end(); ++dirIter, ++fileIter) {
            if (fileIter == file.end() || dirIter->string() != fileIter->string()) {
                return false;
            }
        }
        return true;
    }

    FileParser *contentGenerator::createFileParserOfRegular(const fs::path &file) {
        std::string extension = file.extension().string();
        if (configuration.isScript(extension)) {
            return new FileParserScript(file, configuration.getChunkSize());
        }
        else if (configuration.isText(extension)) {
            return new FileParserText(file, configuration.getChunkSize());
        }
        else {
            // binary is the default mode for unknown types of files
            return new FileParserBinary(file, configuration.getChunkSize());
        }
    }
    FileParser *contentGenerator::createFileParser(const fs::path &file) {

        switch (fs::status(file).type()) {
            case fs::file_type::directory:
                return new FileParserDirectory(file, configuration);
            case fs::file_type::regular:
                return createFileParserOfRegular(file);
            default:
                throw std::runtime_error("Type of file not supported.");
        }
    }


    contentGenerator::contentGenerator(const std::string fileName, const Configuration &configuration)
    : configuration(configuration) {
        fs::path file = fs::canonical(fs::path(fileName));
        if (!isSubdirectory(file, configuration.getRootDirectory())) {
            throw std::runtime_error("Requested file is not a subdirectory of the root.");
        }
        // this line will throw error if the file cannot be opened or doesn't exist
        fileParser = createFileParser(file);
        buildHeaders();
    }
    void contentGenerator::buildHeaders() {
        try {
            size_t contentLength = 0;
            contentLength = fileParser->calculateSize();
            headers["Content-Length"] = std::to_string(contentLength);
        }
        catch (std::runtime_error e) {
            // contentLength couldn't be found, no content-length header was created
        }
    }
    const std::map<std::string, std::string> &contentGenerator::getHeaders() const{
        return headers;
    }

    contentGenerator::~contentGenerator() {
        if (fileParser) {
            delete fileParser;
            fileParser = nullptr;
        }
    }


#endif