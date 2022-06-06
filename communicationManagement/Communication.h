#ifndef HTTP_SERVER_COMMUNICATION_H
#define HTTP_SERVER_COMMUNICATION_H
#include "HttpConnection.h"
#include "../loggable/requests/Request.h"
#include "../loggable/requests/GETRequest.h"
#include "../ContentGenerator.h"
#include "../loggable/responses/SimpleErrorResponse.h"
#include "../exceptions/BadRequest.h"
#include "../exceptions/InternalServerError.h"
#include "../exceptions/MultipleChoices.h"

class Communication {
private:
    HttpConnection &connection;
    Request request;
    const Configuration &configuration;
    //const std::string code = codes::OK;

    Response *createResponse();

public:
    Communication(HttpConnection &connection, const Configuration &configuration)
            : connection(connection), configuration(configuration) {}
    bool recieveRequest();
    void respond();

    void printRequest();
};


#endif //HTTP_SERVER_COMMUNICATION_H
