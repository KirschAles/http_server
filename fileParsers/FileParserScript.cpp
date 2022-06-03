#include "FileParserScript.h"
#include "errno.h"
// the fs::path &path is expected to be already checked for validity
FILE *executeCommand(const std::string &command) {
    return popen(command.c_str(), "r");
}
FileParserScript::FileParserScript(const std::string &command, size_t chunkSize)
        : FileParser(chunkSize) {
    // to execute script we just need to input the name of the scripts file to bash
    output = executeCommand(command);
    if (output == nullptr) {
        throw std::runtime_error("Command failed to execute.");
    }
    updateChunk();
    size = calculateSize();
}
std::string FileParserScript::newChunk() {
    std::string newChunk;
    for (size_t i = 0; i < chunkSize; i++) {
        int byte;
        if ((byte = fgetc(output)) == EOF) {
            return std::move(newChunk);
        }
        newChunk.push_back(static_cast<unsigned char>(byte));
    }
    return std::move(newChunk);
}

void FileParserScript::updateChunk() {
    std::string toReturn = newChunk();
    size = toReturn.length();
    currentChunk = std::move(toReturn);
}
std::string FileParserScript::getChunk() {
    std::string oldChunk = std::move(currentChunk);
    updateChunk();
    return std::move(oldChunk);
}

bool FileParserScript::isEmpty() const {
    return size==0;
}
size_t FileParserScript::calculateSize() const {
    return currentChunk.length();
}
size_t FileParserScript::getSize() const {
    // if the whole script result was taken in one chunk return the content length
    if (size < chunkSize) {
        return size;
    }
    // a part of the result is yet unread and thus we cannot discern the content-length
    throw std::runtime_error("Content-length couldn't be discerned.");
}
FileParserScript::~FileParserScript() {
    if (output) {
        std::fclose(output);
    }
}