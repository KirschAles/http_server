#include "FullResponse.h"
bool FullResponse::sendStatusLine() {
    std::string line = httpVersion + " " + codes::OK + " " + "ok" + "\r\n";
    return connection.send(line);
}
bool FullResponse::sendHeaders(contentGenerator &contentGenerator) {
    return true;
}
bool FullResponse::send(contentGenerator &contentGenerator) {
    return sendStatusLine() && sendHeaders(contentGenerator) && sendBody(contentGenerator);
}