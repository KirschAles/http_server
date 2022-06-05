#include "Communication.h"
Response *createResponse() {
    return nullptr;
}
bool Communication::recieveRequest() {
    std::string requestName = connection.getBytes(3);
    if (requestName == "GET" && connection.getByte() == ' ') {
        request = std::move(GETRequest(connection));
    }
    else {
        return false;
    }
    return true;
}
void Communication::respond() {
    Response *response = nullptr;
    response = createResponse();
    response->send()

}
void Communication::printRequest() {
    request.print();
}