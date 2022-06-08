#include "Communication.h"

Request *Communication::recieveRequest() {
    std::string requestName;
    Request *request = nullptr;
    try {
        requestName = connection.getBytes(4);
    }
    catch (std::runtime_error e) {
        throw new BadRequest("Request is too small.");
    }
    if (requestName == "GET ") {
        request = new GETRequest(connection, httpVersion);
    }
    else {
        throw new BadRequest("Unknown Request type.");
    }
    return request;
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
Response *Communication::createResponse(Request *request) {
    Response *response = nullptr;
    ContentGenerator generator(request->getFileName(), configuration);
    if (request->getVersion() == http::HTTP09) {
        response = new SimpleResponse(connection, generator);
    }
    else {
        response = new FullResponse(connection, generator, configuration);
    }
    return response;

}

bool Communication::communicate() {
    Response *response = nullptr;
    Request *request = nullptr;

    try {
        connection.startRecording();
        request = recieveRequest();
        connection.stopRecording();
        response = createResponse(request);
    }
    catch (HttpException *e) {
        response = createErrorResponse(*e);
    }
    catch (std::runtime_error e) {
        return false;
    }
    response->send();
    if (response) {
        delete response;
    }
    if (request) {
        delete request;
    }
    return true;
}
