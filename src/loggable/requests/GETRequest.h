#ifndef HTTP_SERVER_GETREQUEST_H
#define HTTP_SERVER_GETREQUEST_H
#include "Request.h"
/**
 * GET request,
 *
 * The only thing this class handles right now is the creation of Messages to Logger
 * through it only adds "GET " before the start of the message and lets Response handle the rest
 */
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
