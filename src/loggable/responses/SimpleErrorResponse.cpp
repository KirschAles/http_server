#include "SimpleErrorResponse.h"


std::string SimpleErrorResponse::getFullMessage() {
    return exception.what();
}
std::string SimpleErrorResponse::getPartialMessage() {
    return std::move(getFullMessage());
}