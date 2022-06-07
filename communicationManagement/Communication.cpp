#include "Communication.h"

void Communication::recieveRequest() {
    std::string requestName;
    try {
        requestName = connection.getBytes(4);
    }
    catch (std::runtime_error e) {
        throw new BadRequest("Request is too small.");
    }
    if (requestName == "GET ") {
        request = std::move(GETRequest(connection, httpVersion));
    }
    else {
        throw new BadRequest("Unknown Request type.");
    }
}
Response *Communication::createErrorResponse(HttpException &e) {
    Response *response = nullptr;
    // exception is going to be freed in ErrorResponse destructor
    if (httpVersion == http::HTTP09) {
        response = new SimpleErrorResponse(connection, e);
    }
    else {
        response = new FullErrorResponse(connection, e, http::HTTP10);
    }
    return response;
}
Response *Communication::createResponse() {
    Response *response = nullptr;
    ContentGenerator generator(request.getFileName(), configuration);
    if (request.getVersion() == http::HTTP09) {
        response = new SimpleResponse(connection, generator);
    }
    else {
        response = new FullResponse(connection, generator, configuration);
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
void Communication::communicate() {
    Response *response = nullptr;
    try {
        recieveRequest();
        response = createResponse();
    }
    catch (HttpException *e) {
        response = createErrorResponse(*e);
    }
    response->send();
    response->log(configuration.getLog());
}
void Communication::printRequest() {
    request.print();
}