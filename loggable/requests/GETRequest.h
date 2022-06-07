#ifndef HTTP_SERVER_GETREQUEST_H
#define HTTP_SERVER_GETREQUEST_H
#include "Request.h"

class GETRequest: public Request {
private:
public:
    GETRequest(HttpConnection &connection, std::string &version)
            : Request(connection, version) {
    }
};


#endif //HTTP_SERVER_GETREQUEST_H
