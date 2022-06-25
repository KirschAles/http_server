#include "Request.h"


/**
 *
 * @param connection HttpConnection through which the client sends data
 * @return string new Line
 * @error BadRequest on std::runtime_exception
 * Porpuse of this function is to contain the HttpConnections getLine, but
 * with the exchange of std::runtime_error for BadRequest, so in the case of error
 * Communication manager can deal with it
 *
 * runtime_exception means that the connection ran out of bytes without
 * hitting CRLF which is not possible if the data is entered correctly,
 *
 * only use for dealing with headers and the first line,
 * will through untrue errors if used on the RequestBody
 */
std::string Request::getLineSafely(HttpConnection &connection) {
    try {
        return std::move(connection.getLine());
    }
    catch (const std::runtime_error &e) {
        throw BadRequest("Bad Format");
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

/**
 *
 * @param connection HttpConnection, data comes in through here
 * @param version a reference to http version, here so the outside knows the version of request
 *                  even if the contruction of the other parts of request fail
 */
void Request::setRequestLine(HttpConnection &connection, std::string &version) {
    std::string line = getLineSafely(connection);
    std::string remainingLine = setFileName(line);
    setHttpVersion(remainingLine, version);

}
/**
 *
 * @param line string, must be at least a line long, otherwise the behavour will be undefined
 * @return string returns a header name
 * @error throws badRequest if the header is incorrect
 */
std::string Request::getHeaderName(std::string &line) {
    std::string name;
    char tmp;
    size_t pos;
    for (pos = 0; ':' != (tmp = line[pos]); pos++) {
        // attribute name must be made of printable characters
        if (!std::isprint(tmp)) {
            throw BadRequest("Header name in request is made of non-printable characters.");
        }
        // no whitespace should be between the start of header name and ':'
        else if (std::isspace(tmp)) {
            throw BadRequest("Header name in request contains whitespace characters");
        }
        // the header name must not depend on the case of character
        name += tolower(tmp);
        if (pos == line.length() - 1) {
            throw BadRequest("No ':' character in header");
        }
    }
    // there is one space remaining in line at best
    if (pos >= line.length() - 2) {
        throw BadRequest("No value in header");
    }
    // after ':' must be spacebar
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
/**
 *
 * @param connection HttpConnection, data comes in from there
 * @param version version, used so the communication manager can findout what type of error message can he send if request fails
 */
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