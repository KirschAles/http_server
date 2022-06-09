#include "HttpConnection.h"

/**
 *
 * @param byte char, character that we want to put back in the buffer
 * puts byte back in the buffer
 */
void HttpConnection::rollBackByte(char byte) {
    buffer.push_front(byte);
}
/**
 *
 * @return boolean, true if any bytes were recieved, false otherwise
 * puts new chunk of bytes in the que
 */
bool HttpConnection::newChunk() {
    std::string chunk = connection.recieve(chunkLength);
    for (char byte : chunk) {
        buffer.push_back(byte);
    }
    return chunk.length() > 0;
}

/**
 *
 * @param size number of bytes to assert
 * @return boolean true if there is at least size of bytes remaining, false otherwise
 */
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

/**
 *
 * @return boolean, true if the CSLR is next in line to be read, false otherwise
 * Returns everything to the previous state
 */
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
/**
 *
 * @return char return a byte thats next in line
 * @error std::runtime_error if no byte remains
 * Gets the bytes thats next in the line, if recording is on, the Byte will be also recorded
 */
char HttpConnection::getByte() {
    if (!assertSize(1)) {
        throw std::runtime_error("Ran out of bytes: getByte()");
    }
    char byte = buffer.front();
    buffer.pop_front();
    // add to recording
    // limited so it doesn't hug the memory if for example the message send is really big
    if (isRecording && recording.length() < chunkLength) {
        recording += byte;
    }
    return byte;
}
/**
 *
 * @param length number of bytes to be collected
 * @return string, bytes that are wanted in order
 * @error std::runtime_error if there is not enough bytes
 */
std::string HttpConnection::getBytes(int length) {
    std::string bytes;
    for (int i = 0; i < length; i++) {
        bytes += getByte();
    }
    return std::move(bytes);
}
/**
 *
 * @return string, the whole line
 * @error std::runtime_error if the bytes run out before CSLR is reached
 */
std::string HttpConnection::getLine() {
    std::string line;
    while (!isCSLR()) {
        line += getByte();
    }

    return std::move(line);
}