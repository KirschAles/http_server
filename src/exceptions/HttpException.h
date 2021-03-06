#ifndef HTTP_SERVER_HTTPEXCEPTION_H
#define HTTP_SERVER_HTTPEXCEPTION_H
#include <stdexcept>

/**
 * Base class for Http exceptions
 */
class HttpException: public std::runtime_error {
protected:
    const std::string code;

    HttpException(const std::string &eMessage, const std::string &code): std::runtime_error(eMessage), code(code) {}
    HttpException(const HttpException &error): std::runtime_error(error), code(error.code) {}
    HttpException(HttpException &&error): std::runtime_error(std::move(error)), code(std::move(error.code)) {}
public:
    const std::string &getCode() const {return code;}
    virtual ~HttpException() {}
};
#endif //HTTP_SERVER_HTTPEXCEPTION_H
