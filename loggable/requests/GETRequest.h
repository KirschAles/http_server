#ifndef HTTP_SERVER_GETREQUEST_H
#define HTTP_SERVER_GETREQUEST_H
#include "Request.h"

class GETRequest: public Request {
private:
public:
    GETRequest(HttpConnection &connection, std::string &version)
            : Request(connection, version) {
    }
    std::string getFullMessage() override {return std::move(std::string("GET ") + Request::getFullMessage());}
    std::string getPartialMessage() override {return std::move(std::string("GET ") + Request::getFullMessage());}
};


#endif //HTTP_SERVER_GETREQUEST_H
