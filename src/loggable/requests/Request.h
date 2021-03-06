#ifndef HTTP_SERVER_REQUEST_H
#define HTTP_SERVER_REQUEST_H
#include "../Loggable.h"
#include "../../communicationManagement/HttpConnection.h"
#include <map>
#include <iostream>
#include "../../constants/http.h"
#include "../../exceptions/BadRequest.h"


/**
 * A base for Requests
 *
 * Can be used for easy building of other requests other than GET
 * the classes function is save all informations from request common to all types Requst
 * First line and possibly headers
 */
class Request: public Loggable {
protected:
    std::string httpVersion;
    std::map<std::string, std::string> headers;
    std::string fileName;

    std::string setFileName(const std::string line);
    void setHttpVersion(const std::string &line, std::string &version) {
        // there should be nothing else but the http version here
        // if there is nothing than it means that the request is a simple request
        // and thus http/0.9
        httpVersion = line.length()>0 ? line : http::HTTP09;
        version = httpVersion;
    }
    void setRequestLine(HttpConnection &connection, std::string &version);
    // line must be at least one character long, otherwise undefined behaviour
    std::string getHeaderName(std::string &line);
    void setHeader(std::string &line);
    bool setHeaders(HttpConnection &connection);
    std::string getLineSafely(HttpConnection &connection);

    std::string buildStatusLine();
    std::string buildHeaders();
    virtual std::string buildFullRequest();
public:
    Request() {}
    // On ERROR throws std::runtime_error
    Request(HttpConnection &connection, std::string &version);
    Request(const Request &request)
    : httpVersion(request.httpVersion), headers(request.headers), fileName(request.fileName) {}
    Request(Request &&request)
    : httpVersion(std::move(request.httpVersion)),
      headers(std::move(request.headers)),
      fileName(std::move(request.fileName)) {}
    Request &operator=(const Request &request);
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
    std::string getFullMessage() override {return std::move(buildFullRequest());}
    std::string getPartialMessage() override {return std::move(buildStatusLine());}
    void print();
    virtual ~Request() {}

};


#endif //HTTP_SERVER_REQUEST_H
