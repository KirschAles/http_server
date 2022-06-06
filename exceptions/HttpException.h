#ifndef HTTP_SERVER_HTTPEXCEPTION_H
#define HTTP_SERVER_HTTPEXCEPTION_H
#include <stdexcept>
class HttpException: public std::runtime_error {
    const std::string code;
public:
    HttpException(const std::string &eMessage, const std::string &code): std::runtime_error(eMessage), code(code) {}
    HttpException(const HttpException &error): std::runtime_error(error), code(error.code) {}
    HttpException(HttpException &&error): std::runtime_error(std::move(error)), code(std::move(error.code)) {}
    const std::string &getCode() {return code;}
};
#endif //HTTP_SERVER_HTTPEXCEPTION_H
