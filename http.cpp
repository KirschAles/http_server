
#include "http.h"

void http::Connection::rollBackByte(char byte) {
    buffer.push_front(byte);
}
bool http::Connection::newChunk() {
    std::string chunk = connection.recieve(chunkLength);
    for (char byte : chunk) {
        buffer.push_back(byte);
    }
    return chunk.length() > 0;
}
bool http::Connection::assertSize(size_t size) {
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
bool http::Connection::isCSLR() {
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
char http::Connection::getByte() {
    if (!assertSize(1)) {
        throw std::runtime_error("Ran out of bytes: getByte()");
    }
    char byte = buffer.front();
    buffer.pop_front();
    return byte;
}
std::string http::Connection::getBytes(int length) {
    std::string bytes;
    for (int i = 0; i < length; i++) {
        bytes += getByte();
    }
    return std::move(bytes);
}
std::string http::Connection::getLine() {
    std::string line;
    while (!isCSLR()) {
        line += getByte();
    }
    return std::move(line);
}




std::string http::Request::setFileName(const std::string line) {
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

void http::Request::setRequestLine(Connection &connection) {
    std::string line = connection.getLine();
    std::string remainingLine = setFileName(line);
    setHttpVersion(remainingLine);

}
// line must be at least one character long, otherwise undefined behaviour
std::string http::Request::getHeaderName(std::string &line) {
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
void http::Request::setHeader(std::string &line) {
    std::string name = getHeaderName(line);
    for (size_t pos = 0; pos < line.length(); pos++) {
        headers[name] += line[pos];
    }
}
bool http::Request::setHeaders(Connection &connection) {
    std::string line = connection.getLine();
    while (line.length() != 0) {
        setHeader(line);
        line = connection.getLine();
    }
    return true;
}
http::Request::Request(Connection &connection) {
    setRequestLine(connection);
    setHeaders(connection);
}

http::Request &http::Request::operator=(const Request &request)
{
    fileName = request.fileName;
    httpVersion = request.httpVersion;
    headers = request.headers;
    return *this;
}
void http::Request::log(const fs::path &logFile) {

}
http::Request &http::Request::operator=(http::Request &&request)
{
    fileName = std::move(request.fileName);
    httpVersion = std::move(request.httpVersion);
    headers = std::move(request.headers);
    return *this;
}
void http::Request::print() {
    std::cout << fileName << std::endl << httpVersion << std::endl;
    for (auto i: headers) {
        std::cout << i.first << ": " << i.second << std::endl;
    }
}



void http::Response::log(const fs::path &file) {}


bool http::SimpleResponse::sendBody(contentGenerator &contentGenerator) {
    std::string body;
    while ((body=contentGenerator.getBodyPart()).length() != 0) {
        if (!connection.send(body)) {
            return false;
        }
    }
    return true;
}


void http::SimpleResponse::log(const fs::path &file) {

}



void sendStatusLine();
void send();

bool http::Communication::recieveRequest() {
    std::string requestName = connection.getBytes(3);
    if (requestName == "GET" && connection.getByte() == ' ') {
        request = std::move(GETRequest(connection));
    }
    else {
        return false;
    }
    return true;
}
void http::Communication::respond() {
}
void http::Communication::printRequest() {
    request.print();
}


