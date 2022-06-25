#include "FullErrorResponse.h"

bool FullErrorResponse::sendStatusLine() {
    std::string statusLine = buildStatusLine() + http::CRLF + http::CRLF;
    return connection.send(std::move(statusLine));
}
std::string FullErrorResponse::buildStatusLine() {
    return std::move(version + " " + code + " " + what);
}
/**
 *
 * @return string, The same output as partialMessage
 * Full message and partial message  are the same, because no headers are or body are send
 * in full error messages
 */
std::string FullErrorResponse::getFullMessage() {
    return std::move(getPartialMessage());
}
std::string FullErrorResponse::getPartialMessage() {
    return std::move(buildStatusLine());
}