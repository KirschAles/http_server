#include "SimpleResponse.h"

bool SimpleResponse::sendBody() {
    std::string body;
    while (!contentGenerator.isEmpty()) {
        if (!connection.send(contentGenerator.getChunk())) {
            return false;
        }
    }
    return true;
}


/**
 *
 * @return string partial message intended for Logger
 *
 */
std::string SimpleResponse::getPartialMessage() {
    // returns this, because body is practically unsendable
    // and there is nothing else to log
    return codes::OK;
}
/**
 *
 * @return string intended for logger
 * Only Partial is send, because recording whole body of response would be prohibitevly expensive
 * and this is not the stuff that will cause errors anyway
 */
std::string SimpleResponse::getFullMessage() {
    return std::move(getPartialMessage());
}