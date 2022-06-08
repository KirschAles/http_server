#include "ContentGenerator.h"

namespace fs = std::experimental::filesystem;

    // directory must be directory, should be checked by the configuration
    bool ContentGenerator::isSubdirectory(const fs::path &file, const fs::path &directory) {
        if (fs::status(directory).type() != fs::file_type::directory) {
            // this should be checked by config but better to be safe
            throw new InternalServerError("Root is not Directory");
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
                throw new BadRequest("Type of file not supported.");
        }
    }
    fs::path ContentGenerator::removeDots(const fs::path &file) const {
        fs::path newPath;
        for (auto iterator =  file.begin(); iterator != file.end(); ++iterator) {
            if (*iterator == "~") {
                throw new BadRequest("Bad File Format");
            }
            if (*iterator == "..") {
                newPath.remove_filename();
            }
            else if (*iterator != ".") {
                newPath /= *iterator;
            }
        }
        return newPath;
    }
    bool ContentGenerator::isEqual(const fs::path &file1, const fs::path &file2) const {
        fs::path file1Rel = removeDots(file1.relative_path());
        fs::path file2Rel = removeDots(file2.relative_path());
        auto iterator1 = file1Rel.begin();
        auto iterator2 = file2Rel.begin();

        for (;iterator1 != file1Rel.end() && iterator2 != file2Rel.end(); ++iterator1, ++iterator2) {
            if (*iterator1 != *iterator2) {
                return false;
            }
        }
        return iterator1 == file1Rel.end() && iterator2 == file2Rel.end();
    }
    ContentGenerator::ContentGenerator(const std::string fileName, const Configuration &configuration)
    : configuration(configuration) {
        fs::path file = fs::path(configuration.getRootDirectory().string() + fileName);
        if (isEqual(file, absolute(configuration.getKillFile()))) {
            throw std::runtime_error("Turning off.");
        }
        if (!fs::exists(file)) {
            throw new BadRequest("Requested file doesn't exist.");
        }
        file = fs::canonical(file);
        if (!isSubdirectory(file, configuration.getRootDirectory())) {
            throw new BadRequest("Requested file is not a subdirectory of the root.");
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


