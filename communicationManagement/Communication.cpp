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
Response *Communication::createResponse() {
    Response *response = nullptr;
    try {
        ContentGenerator generator(request.getFileName(), configuration);
        if (request.getVersion() == http::HTTP09) {
            response = new SimpleResponse(connection, generator);
        }
        else {
            response = new FullResponse(connection, generator, configuration);
        }
    }
    catch (HttpException *e) {
        // exception is going to be freed in ErrorResponse destructor
        if (request.getVersion() == http::HTTP09) {
            response = new SimpleErrorResponse(connection, *e);
        }
        else {
            response = new FullErrorResponse(connection, *e, http::HTTP10);
        }
    }
    return response;

}
void Communication::respond() {
    Response *response = nullptr;
    request.getFileName();
    response = createResponse();
    response->send();
    response->log(configuration.getLog());

    if (response) {
        delete response;
    }
}
void Communication::printRequest() {
    request.print();
}