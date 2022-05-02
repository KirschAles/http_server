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
    constexpr char * HTTP09 = "HTTP/0.9";
    constexpr char * HTTP10 = "HTTP/1.0";

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
    class Request {
    protected:
        std::string fileName;
        std::string httpVersion;
        std::map<std::string, std::string> headers;

        std::string setFileName(const std::string line) {
            size_t pos;
            for (pos = 0; line[pos] != ' '; pos++) {
                fileName += line[pos];
                // put here so if there is no ' ' in string,
                // the posisition doesnt go out of bonds
                if (pos == line.length() - 1) {
                    break;
                }
            }
            return std::move(line.substr(++pos));
        }
        void setHttpVersion(const std::string &line) {
            // there should be nothing else but the http version here
            // if there is nothing than it means that the request is a simple request
            // and thus http/0.9
            httpVersion = line.length()>0 ? line : HTTP09;
        }
        void setRequestLine(Connection &connection) {
            std::string line = connection.getLine();
            std::string remainingLine = setFileName(line);
            setHttpVersion(remainingLine);

        }
        // line must be at least one character long, otherwise undefined behaviour
        std::string getHeaderName(std::string &line) {
            std::string name;
            char tmp;
            size_t pos;
            for (pos = 0; ':' != (tmp = line[pos]); pos++) {
                // attribute name must be made of printable characters
                if (!std::isprint(tmp)) {
                    throw std::runtime_error("Header name in request is made of non-printable characters.");
                }
                else if (std::isspace(tmp)) {
                    throw std::runtime_error("Header name in request contains whitespace characters");
                }
                // the header name must not depend on the case of character
                name += tolower(tmp);
                if (pos == line.length() - 1) {
                    throw std::runtime_error("No ':' character in header");
                }
            }
            if (pos >= line.length() - 2) {
                throw std::runtime_error("No value in header");
            }
            if (line[++pos] != ' ') {
                throw std::runtime_error("Bad format of header, missing space after ':'");
            }
            line = line.substr(++pos);
            return std::move(name);
        }
        void setHeader(std::string &line) {
            std::string name = getHeaderName(line);
            for (size_t pos = 0; pos < line.length(); pos++) {
                headers[name] += line[pos];
            }
        }
        bool setHeaders(Connection &connection) {
            std::string line = connection.getLine();
            while (line.length() != 0) {
                setHeader(line);
                line = connection.getLine();
            }
            return true;
        }
    public:
        Request() {}
        // On ERROR throws std::runtime_error
        Request(Connection &connection) {
            setRequestLine(connection);
            setHeaders(connection);
        }
        Request(const Request &request)
        : fileName(request.fileName), httpVersion(request.httpVersion), headers(request.headers) {}
        Request(Request &&request)
        : fileName(std::move(request.fileName)), httpVersion(request.httpVersion), headers(request.headers) {}
        Request &operator=(const Request &request)
        {
            fileName = request.fileName;
            httpVersion = request.httpVersion;
            headers = request.headers;
            return *this;
        }
        Request &operator=(Request &&request)
        {
            fileName = std::move(request.fileName);
            httpVersion = std::move(request.httpVersion);
            headers = std::move(request.headers);
            return *this;
        }

        void print() {
            std::cout << fileName << std::endl << httpVersion << std::endl;
            for (auto i: headers) {
                std::cout << i.first << ": " << i.second << std::endl;
            }
        }

    };
    class GETRequest: public Request {
    private:
    public:
        GETRequest(Connection &connection)
        : Request(connection) {
        }
    };
    class Communication {
    private:
        Connection &connection;
        Request request;

    public:
        Communication(Connection &connection)
                : connection(connection) {}
        bool recieveRequest() {
            std::string requestName = connection.getBytes(3);
            if (requestName == "GET" && connection.getByte() == ' ') {
                request = std::move(GETRequest(connection));
            }
            else {
                return false;
            }
            return true;
        }

        void printRequest() {
            request.print();
        }
    };

}

int main() {


}