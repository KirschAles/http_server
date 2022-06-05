#ifndef HTTP_SERVER_SIMPLERESPONSE_H
#define HTTP_SERVER_SIMPLERESPONSE_H

#include "Response.h"
#include "../../contentGenerator.h"
#include "../../communicationManagement/HttpConnection.h"
// response to a http/0.9 request
class SimpleResponse: public Response {
protected:
    HttpConnection &connection;
    contentGenerator &contentGenerator;
    virtual bool sendBody();
public:
    SimpleResponse(HttpConnection &connection, contentGenerator &contentGenerator)
            : connection(connection), contentGenerator(contentGenerator) {}
    void log(const fs::path &file) override;
    // used like this, so fullResponse can also send headers along with the body
    bool send() override {
        return sendBody();
    }
};


#endif //HTTP_SERVER_SIMPLERESPONSE_H
