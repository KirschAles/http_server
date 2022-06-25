#include "FileParserRegular.h"
FileParserRegular::FileParserRegular(const fs::path &filePath,
                                     size_t chunkSize,
                                     std::ios_base::openmode mode)
        : FileParser(chunkSize), file(filePath, mode), path(filePath) {
    if (file.fail()) {
        std::string error;
        error += "File ";
        error += filePath.string();
        error += " couldn't be opened.";
        throw std::runtime_error(error);
    }
    size = calculateSize();
}
/**
 *
 * @return string, data of the new chunk of data, the string will be empty if no data remains
 * @error std::runtim_error, throws error if the reading from the file fails
 */
std::string FileParserRegular::getChunk() {
    int bytesWanted = chunkSize;
    char *chunk = new char[bytesWanted];
    std::string toBeReturned;
    if (!file.eof()) {
        toBeReturned.reserve(bytesWanted);
        file.read(chunk, sizeof(chunk));
        if (file.bad()) {
            delete[] chunk;
            throw std::runtime_error("Couldn't be read from file.");
        }
        size -= file.gcount();
        toBeReturned.append(chunk, file.gcount());
    }
    delete[] chunk;
    return toBeReturned;
}
size_t FileParserRegular::calculateSize() const {
    return fs::file_size(path);
}
bool FileParserRegular::isEmpty() const {
    return size==0;
}
FileParserRegular::~FileParserRegular() {
    if (file.is_open()) {
        file.close();
    }
}
size_t FileParserRegular::getSize() const {return size;}
