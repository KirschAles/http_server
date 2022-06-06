#ifndef HTTP_SERVER_BADREQUEST_H
#define HTTP_SERVER_BADREQUEST_H
#include <stdexcept>
class BadRequest: public std::runtime_error {
public:
    BadRequest(const std::string &eMessage): std::runtime_error(eMessage) {}
    BadRequest(const BadRequest &error): std::runtime_error(error) {}
    BadRequest(BadRequest &&error): std::runtime_error(std::move(error)) {}
    BadRequest &operator=(const BadRequest &error) = default;

};
#endif //HTTP_SERVER_BADREQUEST_H
