#ifndef HTTP_SERVER_SIMPLERESPONSE_H
#define HTTP_SERVER_SIMPLERESPONSE_H

#include "Response.h"
#include "../../ContentGenerator.h"
#include "../../communicationManagement/HttpConnection.h"

/**
 * A response to a HTTP/0.9 request
 */
class SimpleResponse: public Response {
protected:
    ContentGenerator contentGenerator;
    virtual bool sendBody();
public:
    SimpleResponse(HttpConnection &connection, ContentGenerator &contentGenerator)
            : Response(connection), contentGenerator(std::move(contentGenerator)) {}

    // used like this, so fullResponse can also send headers along with the body
    bool send() override {
        return sendBody();
    }
    std::string getFullMessage() override;
    std::string getPartialMessage() override;
};


#endif //HTTP_SERVER_SIMPLERESPONSE_H
