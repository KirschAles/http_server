#include "HttpConnection.h"

void HttpConnection::rollBackByte(char byte) {
    buffer.push_front(byte);
}
bool HttpConnection::newChunk() {
    std::string chunk = connection.recieve(chunkLength);
    for (char byte : chunk) {
        buffer.push_back(byte);
    }
    return chunk.length() > 0;
}
bool HttpConnection::assertSize(size_t size) {
    if (size > chunkLength) {
        throw std::runtime_error("Wanted size to assert is too big.");
    }
    if (buffer.size() < size) {
        newChunk();
        if (buffer.size() < size) {
            return false;
        }
    }
    return true;
}
// checks if there is a CSLR at the start, on succes deletes the CSLR from the start
// else return false and keeps it in the previous state
bool HttpConnection::isCSLR() {
    if (!assertSize(1)) {
        return false;
    }
    // the first char of CSLR is either \r or \n,
    // if it is not \r it must \n
    if ('\n' == buffer.front()) {
        buffer.pop_front();
        return true;
    }
    if(!assertSize(2)) {
        return false;
    }
    if ('\r' == buffer.front()) {
        char tmp = buffer.front();
        buffer.pop_front();
        if ('\n' == buffer.front()) {
            buffer.pop_front();
            return true;
        }
        buffer.push_front(tmp);
    }
    return false;
}
char HttpConnection::getByte() {
    if (!assertSize(1)) {
        throw std::runtime_error("Ran out of bytes: getByte()");
    }
    char byte = buffer.front();
    //std::cout << byte;
    buffer.pop_front();

    return byte;
}
std::string HttpConnection::getBytes(int length) {
    std::string bytes;
    for (int i = 0; i < length; i++) {
        bytes += getByte();
    }
    return std::move(bytes);
}
std::string HttpConnection::getLine() {
    std::string line;
    while (!isCSLR()) {
        line += getByte();
    }

    return std::move(line);
}