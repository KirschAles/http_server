#ifndef HTTP_SERVER_COMMUNICATION_H
#define HTTP_SERVER_COMMUNICATION_H
#include "HttpConnection.h"
#include "../loggable/requests/Request.h"
#include "../loggable/requests/GETRequest.h"
#include "../constants/codes.h"
class Communication {
private:
    HttpConnection &connection;
    Request request;
    const Configuration &configuration;
    const std::string code = codes::OK;

public:
    Communication(HttpConnection &connection, const Configuration &configuration)
            : connection(connection), configuration(configuration) {}
    bool recieveRequest();
    void respond();
    void printRequest();
};


#endif //HTTP_SERVER_COMMUNICATION_H
