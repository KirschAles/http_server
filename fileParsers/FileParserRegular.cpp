#include "FileParserRegular.h"
FileParserRegular::FileParserRegular(const fs::path &filePath,
                                     size_t chunkSize,
                                     std::ios_base::openmode mode)
        : FileParser(chunkSize), path(filePath), file(filePath, mode) {
    if (file.fail()) {
        std::string error;
        error += "File ";
        error += filePath.string();
        error += " couldn't be opened.";
        throw std::runtime_error(error);
    }
    size = calculateSize();
}
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