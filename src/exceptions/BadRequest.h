#ifndef HTTP_SERVER_BADREQUEST_H
#define HTTP_SERVER_BADREQUEST_H
#include "HttpException.h"
#include "../constants/codes.h"
class BadRequest: public HttpException {
public:
    BadRequest(const std::string &eMessage): HttpException(eMessage, codes::BadRequest) {}
    BadRequest(const BadRequest &error): HttpException(error) {}
    BadRequest(BadRequest &&error): HttpException(std::move(error)) {}
};
#endif //HTTP_SERVER_BADREQUEST_H
