#include "FullErrorResponse.h"

bool FullErrorResponse::sendStatusLine() {
    std::string statusLine = version + " " + exception.getCode() + " " + exception.what() + http::CRLF;
    return connection.send(std::move(statusLine));
}