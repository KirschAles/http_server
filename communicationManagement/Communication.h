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

class Communication {
private:
    HttpConnection &connection;
    Request request;
    const Configuration &configuration;
    std::string httpVersion;

    Response *createResponse();
    Response *createErrorResponse(HttpException &e);
public:
    Communication(HttpConnection &connection, const Configuration &configuration)
            : connection(connection), configuration(configuration), httpVersion(http::HTTP09) {}
    void recieveRequest();
    void respond();
    bool communicate();

    void printRequest();
};


#endif //HTTP_SERVER_COMMUNICATION_H
