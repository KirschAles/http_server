#ifndef HTTP_SERVER_FULLERRORRESPONSE_H
#define HTTP_SERVER_FULLERRORRESPONSE_H
#include "SimpleErrorResponse.h"
#include "../../constants/http.h"
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
