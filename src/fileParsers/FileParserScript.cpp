#include "FileParserScript.h"
#include "errno.h"
/**
 *
 * @param command usually file where the script is, should be checked already for the validity
 * @return FILE *, pointer to a pipe with the output from the script, nullptr if the command fails
 */
FILE *executeCommand(const std::string &command) {
    return popen(command.c_str(), "r");
}
/**
 *
 * @param command string, command to execute, already checked for validity as file by ContentGenerator
 * @param chunkSize size_t numberOfBytes the programm can hold in it's buffer
 * @error std::runtime_error if the command failes to execute
 */
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
/**
 *
 * @return string, gets a newPart of buffer from the pipe
 */
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

/**
 * Used for getting new chunks
 * size is set by the newChunk because
 * there is no way to find out the size of the whole pipe,
 * without saving it all in the buffer.
 * If the size is less than the maximal length of the buffer
 * the size may be used further.
 *
 */
void FileParserScript::updateChunk() {
    std::string toReturn = newChunk();
    size = toReturn.length();
    currentChunk = std::move(toReturn);
}

/**
 *
 * @return string, returns next chunk
 * Function returns alredy saved chunk, and than saves new one into buffer
 */
std::string FileParserScript::getChunk() {
    std::string oldChunk = std::move(currentChunk);
    updateChunk();
    return std::move(oldChunk);
}

/**
 *
 * @return boolean, true if empty, false otherwise
 * Checks if the buffer is empty, due to the continual nature of the chunks updating
 * this will return true only if the whole pipe was used up.
 */
bool FileParserScript::isEmpty() const {
    return size==0;
}
size_t FileParserScript::calculateSize() const {
    return currentChunk.length();
}

/**
 *
 * @return size_t size of pipe
 * @error runtime_error size couldn't be found out
 * Method returns the number of bytes gotten if it is lower than
 * the size of buffer.
 * It will through error otherwise because than it is no know if the whole output
 * of pipe was read, and thus the size is not a surely what size says
 *
 * The method should be called before the use of getChunk,
 * otherwise the size returned size will be meaningless.
 */
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
        pclose(output);
    }
}