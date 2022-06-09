#include "Communication.h"
/**
 *
 * @return Request * to a newly created request
 * @error BadRequest if the Request fails to be created
 */
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

/**
 *
 * @param e HttpException & that was thrown, when the Requst creation failed
 * @return ErrorResponse *
 */
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
/**
 *
 * @param request Request * a valid request
 * @return Response * a Response that was generated in accordance with the request
 */
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

/**
 *
 * @param logger Logger & object that is used to write exchange info into logfile
 * @return boolean true if the server is supposed to continue running, false if the server must turn off
 * This method facilitates exchange between client and server if the client enters file to shutdown server.
 * Server returns no message back to the client and quietly shutsdown.
 */
bool Communication::communicate(Logger &logger) {
    Response *response = nullptr;
    Request *request = nullptr;

    try {
        connection.startRecording();
        request = recieveRequest();
        connection.stopRecording();
        response = createResponse(request);

        logger.log(request->getPartialMessage(), connection.getIpAddress(), connection.getDomain());
        logger.log(response->getPartialMessage(), connection.getIpAddress(), connection.getDomain());
    }
    catch (HttpException *e) {
        connection.stopRecording();
        response = createErrorResponse(*e);
        logger.log(connection.getRecords(), connection.getIpAddress(), connection.getDomain());
        logger.log(response->getFullMessage(), connection.getIpAddress(), connection.getDomain());
    }
    catch (std::runtime_error e) {
        if (response) {
            delete response;
        }
        if (request) {
            delete request;
        }
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
