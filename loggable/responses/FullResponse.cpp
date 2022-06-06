#include "FullResponse.h"
bool FullResponse::sendStatusLine() {
    std::string line = httpVersion + " " + codes::OK + " " + "ok" + http::CRLF;
    return connection.send(line);
}
// TO DO: need to also implement sending of general headers, not only entity ones
bool FullResponse::sendHeaders() {
    std::string headers;
    for (auto &headerValuePair: contentGenerator.getHeaders()) {
        headers += headerValuePair.first + ": " + headerValuePair.second + http::CRLF;
    }
    headers += http::CRLF;
    return connection.send(headers);
}
bool FullResponse::send() {
    return sendStatusLine() && sendHeaders() && sendBody();
}