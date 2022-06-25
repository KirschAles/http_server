#include "SimpleErrorResponse.h"


SimpleErrorResponse::SimpleErrorResponse(HttpConnection &connection, const HttpException &exception)
: Response(connection), what(exception.what()) {}

bool SimpleErrorResponse::send() {
    return connection.send(what);
}

std::string SimpleErrorResponse::getFullMessage() {
    return what;
}
std::string SimpleErrorResponse::getPartialMessage() {
    return std::move(getFullMessage());
}