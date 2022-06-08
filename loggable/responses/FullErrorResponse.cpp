#include "FullErrorResponse.h"

bool FullErrorResponse::sendStatusLine() {
    std::string statusLine = buildStatusLine() + http::CRLF + http::CRLF;
    return connection.send(std::move(statusLine));
}
std::string FullErrorResponse::buildStatusLine() {
    return std::move(version + " " + exception.getCode() + " " + exception.what());
}
std::string FullErrorResponse::getFullMessage() {
    return std::move(getPartialMessage());
}
std::string FullErrorResponse::getPartialMessage() {
    return std::move(buildStatusLine());
}