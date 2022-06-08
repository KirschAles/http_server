#include "Request.h"

std::string Request::getLineSafely(HttpConnection &connection) {
    try {
        return std::move(connection.getLine());
    }
    catch (std::runtime_error e) {
        throw new BadRequest("Bad Format");
    }
}
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

void Request::setRequestLine(HttpConnection &connection, std::string &version) {
    std::string line = getLineSafely(connection);
    std::string remainingLine = setFileName(line);
    setHttpVersion(remainingLine, version);

}
// line must be at least one character long, otherwise undefined behaviour
std::string Request::getHeaderName(std::string &line) {
    std::string name;
    char tmp;
    size_t pos;
    for (pos = 0; ':' != (tmp = line[pos]); pos++) {
        // attribute name must be made of printable characters
        if (!std::isprint(tmp)) {
            throw new BadRequest("Header name in request is made of non-printable characters.");
        }
        else if (std::isspace(tmp)) {
            throw new BadRequest("Header name in request contains whitespace characters");
        }
        // the header name must not depend on the case of character
        name += tolower(tmp);
        if (pos == line.length() - 1) {
            throw new BadRequest("No ':' character in header");
        }
    }
    if (pos >= line.length() - 2) {
        throw new BadRequest("No value in header");
    }
    if (line[++pos] != ' ') {
        throw BadRequest("Bad format of header, missing space after ':'");
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
    std::string line = getLineSafely(connection);
    while (line.length() != 0) {
        setHeader(line);
        line = getLineSafely(connection);
    }
    return true;
}
Request::Request(HttpConnection &connection, std::string &version) {
    setRequestLine(connection, version);
    setHeaders(connection);
}

Request &Request::operator=(const Request &request)
{
    fileName = request.fileName;
    httpVersion = request.httpVersion;
    headers = request.headers;
    return *this;
}

Request &Request::operator=(Request &&request)
{
    fileName = std::move(request.fileName);
    httpVersion = std::move(request.httpVersion);
    headers = std::move(request.headers);
    return *this;
}
std::string Request::buildStatusLine() {
    return std::move(fileName + " " + httpVersion);
}
std::string Request::buildHeaders() {
    std::string headerInfo;
    for (auto i: headers) {
        headerInfo += i.first + ": " + i.second + http::CRLF;
    }
    return std::move(headerInfo);
}
std::string Request::buildFullRequest() {
    return std::move(buildStatusLine() + http::CRLF + buildHeaders());
}
void Request::print() {
    std::cout << buildFullRequest();
}