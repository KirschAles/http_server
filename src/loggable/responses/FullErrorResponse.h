#ifndef HTTP_SERVER_FULLERRORRESPONSE_H
#define HTTP_SERVER_FULLERRORRESPONSE_H
#include "SimpleErrorResponse.h"
#include "../../constants/http.h"

/**
 * Error Response to HTTP/1.0 or higher request
 *
 * Sends only a status line, no headers are created
 */
class FullErrorResponse: public SimpleErrorResponse {
protected:
    const std::string &version;
    virtual bool sendStatusLine();
    std::string buildStatusLine();

public:
    FullErrorResponse(HttpConnection &connection, const HttpException &exception, const std::string &version)
            : SimpleErrorResponse(connection, exception), version(version) {}

    bool send() override {
        return sendStatusLine();
    }
    std::string getFullMessage() override;
    std::string getPartialMessage() override;
};


#endif //HTTP_SERVER_FULLERRORRESPONSE_H
