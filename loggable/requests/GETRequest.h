#ifndef HTTP_SERVER_GETREQUEST_H
#define HTTP_SERVER_GETREQUEST_H
#include "Request.h"

class GETRequest: public Request {
private:
public:
    GETRequest(HttpConnection &connection)
            : Request(connection) {
    }
};


#endif //HTTP_SERVER_GETREQUEST_H
