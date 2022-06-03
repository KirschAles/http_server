#include "Request.h"
std::string Request::setFileName(const std::string line) {
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

void Request::setRequestLine(HttpConnection &connection) {
    std::string line = connection.getLine();
    std::string remainingLine = setFileName(line);
    setHttpVersion(remainingLine);

}
// line must be at least one character long, otherwise undefined behaviour
std::string Request::getHeaderName(std::string &line) {
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
void Request::setHeader(std::string &line) {
    std::string name = getHeaderName(line);
    for (size_t pos = 0; pos < line.length(); pos++) {
        headers[name] += line[pos];
    }
}
bool Request::setHeaders(HttpConnection &connection) {
    std::string line = connection.getLine();
    while (line.length() != 0) {
        setHeader(line);
        line = connection.getLine();
    }
    return true;
}
Request::Request(HttpConnection &connection) {
    setRequestLine(connection);
    setHeaders(connection);
}

Request &Request::operator=(const Request &request)
{
    fileName = request.fileName;
    httpVersion = request.httpVersion;
    headers = request.headers;
    return *this;
}
void Request::log(const fs::path &logFile) {

}
Request &Request::operator=(Request &&request)
{
    fileName = std::move(request.fileName);
    httpVersion = std::move(request.httpVersion);
    headers = std::move(request.headers);
    return *this;
}
void Request::print() {
    std::cout << fileName << std::endl << httpVersion << std::endl;
    for (auto i: headers) {
        std::cout << i.first << ": " << i.second << std::endl;
    }
}