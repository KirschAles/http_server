#include "ContentGenerator.h"

namespace fs = std::experimental::filesystem;

    /**
     *
     * @param file fs::path in canonical form
     * @param directory fs::path should be at this point checked that this file exists and also in canonical form
     * @return true if file is subdirectory of directory, false otherwise
     */

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
    /**
     *
     * @param file
     * @return FileParser of non-directory
     * Creates a FileParser to care for the file
     * Allocated memory will be freed by ContentGenerators destructor
     */
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

    /**
     *
     * @param file file::path which is to be stripped of .. and .
     * @return FileParser
     * @error BadRequest if there is /~/ in the path, is allowable because this type of stuff is not
     *         expected or even wanted on http server
     */
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

    /**
     *
     * @param file1 filesystem path
     * @param file2 filesystem path
     * @return true if they are equal, false otherwise
     * this function was made, because sometimes it is needed to know
     * if file paths are equal, even when the files don't exist, which makes
     * filesystems functions unusable
     */
    bool ContentGenerator::isEqual(const fs::path &file1, const fs::path &file2) const {
        // makes both paths canonical
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

    /**
     *
     * @param fileName string name of the path
     * @param configuration
     * @error std::runtime_error if shutdown file was asked for, programm starts shutting down
     * @error BadRequest on fail, if either the file doesn't exist or is out of allowed area
     *
     * creates the fileParser if possible, and builds up the entity headers for use by other programms
     */
    ContentGenerator::ContentGenerator(const std::string fileName, const Configuration &configuration)
    : configuration(configuration), fileName(fileName) {
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

    /**
     * builds headers to be used as entity header
     * right now only content-length
     */
    void ContentGenerator::buildHeaders() {
        try {
            size_t contentLength = 0;
            contentLength = fileParser->getSize();
            headers["Content-Length"] = std::to_string(contentLength);
        }
        catch (std::runtime_error e) {
            // contentLength couldn't be found, no content-length header was created
        }
        headers["Content-Type"] = fileParser->getType() + std::string(fileName.extension()).substr(1);
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


