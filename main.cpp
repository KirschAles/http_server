#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <map>
#include <cctype>
#include <deque>
#include "configuration.h"
#include "networking.h"



namespace http {

    // class for http Connection
    class Connection {
    private:
        const ::Connection &connection;
        std::deque<char> buffer;
        size_t chunkLength = 500;
    public:
        Connection(const ::Connection &connection)
        : connection(connection) {}
        void rollBackByte(char byte) {
            buffer.push_front(byte);
        }
        bool newChunk() {
            std::string chunk = connection.recieve(chunkLength);
            for (char byte : chunk) {
                buffer.push_back(byte);
            }
            return chunk.length() > 0;
        }
        bool assertSize(size_t size) {
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
        bool isCSLR() {
            if (!assertSize(1)) {
                throw std::runtime_error("Ran out of bytes: isCSLR()");
            }
            // the first char of CSLR is either \r or \n,
            // if it is not \r it must \n
            if ('\r' != buffer.front()) {
                if ('\n' == buffer.front()) {
                    buffer.pop_front();
                    return true;
                }
                return false;
            }

            if ('\r' == buffer.front()) {
                // make sure that there is enough characters in the buffer
                if (!assertSize(2)) {
                    throw std::runtime_error("Ran out of bytes: isCSLR()");
                }
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
        char getByte() {
            if (!assertSize(1)) {
                throw std::runtime_error("Ran out of bytes: getByte()");
            }
            char byte = buffer.front();
            std::cout << byte;
            buffer.pop_front();
            return byte;
        }
        std::string getBytes(int length) {
            std::string bytes;
            for (int i = 0; i < length; i++) {
                bytes += getByte();
            }
            return std::move(bytes);
        }
        std::string getLine() {
            std::string line;
            while (!isCSLR()) {
                line += getByte();
            }
            return std::move(line);
        }
    };
}

int main() {


}