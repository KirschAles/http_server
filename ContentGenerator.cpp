#include "ContentGenerator.h"

namespace fs = std::experimental::filesystem;

    // directory must be directory, should be checked by the configuration
    bool ContentGenerator::isSubdirectory(const fs::path &file, const fs::path &directory) {
        if (fs::status(directory).type() != fs::file_type::directory) {
            // this should be checked by config but better to be safe
            throw InternalServerError("Root is not Directory");
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

    FileParser *ContentGenerator::createFileParserOfRegular(const fs::path &file) {
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
    FileParser *ContentGenerator::createFileParser(const fs::path &file) {
        switch (fs::status(file).type()) {
            case fs::file_type::directory:
                return new FileParserDirectory(file, configuration.getChunkSize());
            case fs::file_type::regular:
                return createFileParserOfRegular(file);
            default:
                throw BadRequest("Type of file not supported.");
        }
    }


    ContentGenerator::ContentGenerator(const std::string fileName, const Configuration &configuration)
    : configuration(configuration) {
        fs::path file = fs::path(configuration.getRootDirectory().string() + fileName);
        if (!fs::exists(file)) {
            throw BadRequest("Requested file doesn't exist.");
        }
        file = fs::canonical(file);
        if (!isSubdirectory(file, configuration.getRootDirectory())) {
            throw BadRequest("Requested file is not a subdirectory of the root.");
        }
        // this line will throw error if the file cannot be opened or doesn't exist
        fileParser = createFileParser(file);
        buildHeaders();
    }
    void ContentGenerator::buildHeaders() {
        try {
            size_t contentLength = 0;
            contentLength = fileParser->getSize();
            headers["Content-Length"] = std::to_string(contentLength);
        }
        catch (std::runtime_error e) {
            // contentLength couldn't be found, no content-length header was created
        }
    }
    const std::map<std::string, std::string> &ContentGenerator::getHeaders() const{
        return headers;
    }

    ContentGenerator::~ContentGenerator() {
        if (fileParser) {
            delete fileParser;
            fileParser = nullptr;
        }
    }


