#ifndef HTTP_SERVER_REQUEST_H
#define HTTP_SERVER_REQUEST_H
#include "../Loggable.h"
#include "../../communicationManagement/HttpConnection.h"
#include <map>
#include <iostream>
#include "../../constants/http.h"
class Request: public Loggable {
protected:
    std::string httpVersion;
    std::map<std::string, std::string> headers;
    std::string fileName;

    std::string setFileName(const std::string line);
    void setHttpVersion(const std::string &line) {
        // there should be nothing else but the http version here
        // if there is nothing than it means that the request is a simple request
        // and thus http/0.9
        httpVersion = line.length()>0 ? line : http::HTTP09;
    }
    void setRequestLine(HttpConnection &connection);
    // line must be at least one character long, otherwise undefined behaviour
    std::string getHeaderName(std::string &line);
    void setHeader(std::string &line);
    bool setHeaders(HttpConnection &connection);
public:
    Request() {}
    // On ERROR throws std::runtime_error
    Request(HttpConnection &connection);
    Request(const Request &request)
            : fileName(request.fileName), httpVersion(request.httpVersion), headers(request.headers) {}
    Request(Request &&request)
            : fileName(std::move(request.fileName)), httpVersion(request.httpVersion), headers(request.headers) {}
    Request &operator=(const Request &request);
    void log(const fs::path &logFile) override ;
    Request &operator=(Request &&request);
    const std::string &getFileName() {
        return fileName;
    }
    const std::map<std::string, std::string> &getHeaders() {
        return headers;
    }
    const std::string &getVersion() {
        return httpVersion;
    }
    void print();

};


#endif //HTTP_SERVER_REQUEST_H
