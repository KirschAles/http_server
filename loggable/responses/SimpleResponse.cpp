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



std::string SimpleResponse::getPartialMessage() {
    // returns this, because body is practically unsendable
    // and there is nothing else to log
    return codes::OK;
}
std::string SimpleResponse::getFullMessage() {
    return std::move(getPartialMessage());
}