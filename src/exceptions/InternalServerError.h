#ifndef HTTP_SERVER_INTERNALSERVERERROR_H
#define HTTP_SERVER_INTERNALSERVERERROR_H
#include "HttpException.h"
#include "../constants/codes.h"

class InternalServerError: public HttpException {
public:
    InternalServerError(const std::string &eMessage): HttpException(eMessage, codes::InternalServerError) {}
    InternalServerError(const InternalServerError &error): HttpException(error) {}
    InternalServerError(InternalServerError &&error): HttpException(std::move(error)) {}
};
#endif //HTTP_SERVER_INTERNALSERVERERROR_H
