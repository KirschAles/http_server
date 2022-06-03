#ifndef HTTP_SERVER_SIMPLERESPONSE_H
#define HTTP_SERVER_SIMPLERESPONSE_H

#include "../Loggable.h"
#include "../../contentGenerator.h"
#include "../../communicationManagement/HttpConnection.h"
// response to a http/0.9 request
class SimpleResponse: public Loggable {
protected:
    HttpConnection &connection;

    virtual bool sendBody(contentGenerator &contentGenerator);
public:
    SimpleResponse(HttpConnection &connection)
            : connection(connection) {}
    void log(const fs::path &file) override;
    // used like this, so fullResponse can also send headers along with the body
    virtual bool send(contentGenerator &contentGenerator) {
        return sendBody(contentGenerator);
    }
};


#endif //HTTP_SERVER_SIMPLERESPONSE_H