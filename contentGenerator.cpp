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
    // TO DO: replace this with endings that should be loaded from configuration file
    //        this is just a temporary solution
    FileParser *contentGenerator::createFileParserOfRegular(const fs::path &file) {
        std::string extension = file.extension().string();
        if (extension == ".sh" || extension == ".py" || extension == ".exe" || extension == ".out") {
            return new FileParserScript(file, configuration);
        }
        else if (extension == ".txt", extension == ".html", extension == ".cpp", extension == ".h") {
            return new FileParserText(file, configuration);
        }
        else {
            return new FileParserBinary(file, configuration);
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
        fileParser = createFileParser(file);
    }
    std::map<std::string, std::string> contentGenerator::getHeaders() {
        std::map<std::string, std::string> headers;
        headers["Content-Length"] = std::to_string(fileParser->calculateSize());
        return std::move(headers);
    }
    std::string contentGenerator::getBodyPart() {
        return std::move(fileParser->getChunk());
    }
    contentGenerator::~contentGenerator() {
        if (fileParser) {
            delete fileParser;
        }
    }


#endif