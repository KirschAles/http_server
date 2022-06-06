#ifndef HTTP_SERVER_INTERNALSERVERERROR_H
#define HTTP_SERVER_INTERNALSERVERERROR_H
#include <stdexcept>

class InternalServerError: public std::runtime_error {
public:
    InternalServerError(const std::string &eMessage): std::runtime_error(eMessage) {}
    InternalServerError(const InternalServerError &error): std::runtime_error(error) {}
    InternalServerError(InternalServerError &&error): std::runtime_error(std::move(error)) {}
    InternalServerError &operator=(const InternalServerError &error) = default;
};
#endif //HTTP_SERVER_INTERNALSERVERERROR_H
