#ifndef HTTP_SERVER_COMMUNICATION_H
#define HTTP_SERVER_COMMUNICATION_H
#include "HttpConnection.h"
#include "../loggable/requests/Request.h"
#include "../loggable/requests/GETRequest.h"
#include "../ContentGenerator.h"
#include "../loggable/responses/SimpleResponse.h"
#include "../loggable/responses/FullResponse.h"
#include "../loggable/responses/FullErrorResponse.h"
#include "../loggable/responses/SimpleErrorResponse.h"
#include "../exceptions/BadRequest.h"
#include "../exceptions/InternalServerError.h"
#include "../exceptions/MultipleChoices.h"
#include "../Logger.h"

/**
 * Communication
 *
 * This class manages the whole interaction between one server and one client
 * closes Connection after one Request, Response Exchange
 */
class Communication {
private:
    HttpConnection &connection;
    const Configuration &configuration;
    std::string httpVersion;

    Response *createResponse(Request *request);
    Response *createErrorResponse(HttpException &e);
public:
    /**
     *
     * @param connection HttpConnection over which the exchange will be made
     * @param configuration
     * httpVersion is set as HTTP09 so if no information about version of the client
     * can be retrieved, the server will still send response in a format supported by the client.
     */
    Communication(HttpConnection &connection, const Configuration &configuration)
            : connection(connection), configuration(configuration), httpVersion(http::HTTP09) {}
    Request *recieveRequest();
    bool communicate(Logger &logger);

};


#endif //HTTP_SERVER_COMMUNICATION_H
