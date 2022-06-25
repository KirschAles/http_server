#include "SimpleErrorResponse.h"


std::string SimpleErrorResponse::getFullMessage() {
    return what;
}
std::string SimpleErrorResponse::getPartialMessage() {
    return std::move(getFullMessage());
}