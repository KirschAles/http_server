#ifndef HTTP_SERVER_SIMPLEERRORRESPONSE_H
#define HTTP_SERVER_SIMPLEERRORRESPONSE_H
#include "Response.h"
#include "../../exceptions/HttpException.h"

class SimpleErrorResponse: public Response {
protected:
    const HttpException &exception;
public:
    SimpleErrorResponse(HttpConnection &connection, const HttpException &exception)
    : Response(connection), exception(exception) {}

    bool send() override {
        return connection.send(exception.what());
    }
    ~SimpleErrorResponse() override {
        if (&exception) {
            delete &exception;
        }
    }

    virtual std::string getFullMessage();
    std::string getPartialMessage() override;

};



#endif //HTTP_SERVER_SIMPLEERRORRESPONSE_H
