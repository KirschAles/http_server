#include "FullResponse.h"
std::string FullResponse::buildStatusLine() const {
    return std::move(httpVersion + " " + codes::OK + " " + "ok");
}
bool FullResponse::sendStatusLine() {
    std::string line = buildStatusLine() + http::CRLF;
    return connection.send(line);
}
// TO DO: need to also implement sending of general headers, not only entity ones

std::string FullResponse::buildHeaders() const {
    std::string headers;
    for (auto &headerValuePair: contentGenerator.getHeaders()) {
        headers += headerValuePair.first + ": " + headerValuePair.second + http::CRLF;
    }
    return std::move(headers);
}
bool FullResponse::sendHeaders() {
    return connection.send(buildHeaders() + http::CRLF);
}
bool FullResponse::send() {
    return sendStatusLine() && sendHeaders() && sendBody();
}

std::string FullResponse::getPartialMessage() {
    return std::move(buildStatusLine());
}
std::string FullResponse::getFullMessage() {
    return std::move(buildStatusLine() + http::CRLF + buildHeaders());
}
