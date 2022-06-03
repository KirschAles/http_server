#include "SimpleResponse.h"

bool SimpleResponse::sendBody(contentGenerator &contentGenerator) {
    std::string body;
    while (!contentGenerator.isEmpty()) {
        if (!connection.send(contentGenerator.getChunk())) {
            return false;
        }
    }
    return true;
}


void SimpleResponse::log(const fs::path &file) {

}