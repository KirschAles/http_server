#include "Communication.h"

Communication::Communication(HttpConnection &connection, const Configuration &configuration)
: connection(connection), configuration(configuration), httpVersion(http::HTTP09) {}


/**
 *
 * @return Request * to a newly created request
 * @error BadRequest if the Request fails to be created
 */
std::unique_ptr<Request> Communication::recieveRequest() {
    std::string requestName;
    std::unique_ptr<Request> request;
    try {
        requestName = connection.getBytes(4);
    }
    catch (std::runtime_error e) {
        throw BadRequest("Request is too small.");
    }
    if (requestName == "GET ") {
        request = std::make_unique<GETRequest>(connection, httpVersion);
    }
    else {
        throw BadRequest("Unknown Request type.");
    }
    return std::move(request);
}

/**
 *
 * @param e HttpException & that was thrown, when the Requst creation failed
 * @return ErrorResponse *
 */
std::unique_ptr<Response> Communication::createErrorResponse(const HttpException &e) {
    std::unique_ptr<Response> response;
    // exception is going to be freed in ErrorResponse destructor
    if (httpVersion == http::HTTP09) {
        response = std::make_unique<SimpleErrorResponse>(connection, e);
    }
    else {
        response = std::make_unique<FullErrorResponse>(connection, e, http::HTTP10);
    }
    return response;
}
/**
 *
 * @param request Request * a valid request
 * @return Response * a Response that was generated in accordance with the request
 */
std::unique_ptr<Response> Communication::createResponse(Request &request) {
    std::unique_ptr<Response> response;
    ContentGenerator generator(request.getFileName(), configuration);
    if (request.getVersion() == http::HTTP09) {
        response = std::make_unique<SimpleResponse>(connection, generator);
    }
    else {
        response = std::make_unique<FullResponse>(connection, generator, configuration);
    }
    return std::move(response);

}
std::mutex logLock;
/**
 *
 * @param logger Logger & object that is used to write exchange info into logfile
 * @return boolean true if the server is supposed to continue running, false if the server must turn off
 * This method facilitates exchange between client and server if the client enters file to shutdown server.
 * Server returns no message back to the client and quietly shutsdown.
 */
bool Communication::communicate(Logger &logger) {
    std::unique_ptr<Response> response;
    std::unique_ptr<Request> request ;

    try {
        connection.startRecording();
        request = recieveRequest();
        connection.stopRecording();
        response = createResponse(*request);

        std::lock_guard<std::mutex> loggingLock(logLock);
        logger.log(request->getPartialMessage(), connection.getIpAddress(), connection.getDomain());
        logger.log(response->getPartialMessage(), connection.getIpAddress(), connection.getDomain());
    }
    catch (const HttpException &e) {
        connection.stopRecording();
        response = createErrorResponse(e);

        std::lock_guard<std::mutex> loggingLock(logLock);
        logger.log(connection.getRecords(), connection.getIpAddress(), connection.getDomain());
        logger.log(response->getFullMessage(), connection.getIpAddress(), connection.getDomain());
    }
    catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    response->send();
    return true;
}
