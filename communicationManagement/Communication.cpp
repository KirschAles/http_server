#include "Communication.h"

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
}
void Communication::printRequest() {
    request.print();
}