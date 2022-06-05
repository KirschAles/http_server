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


void SimpleResponse::log(const fs::path &file) {

}