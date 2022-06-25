#ifndef HTTP_SERVER_SIMPLEERRORRESPONSE_H
#define HTTP_SERVER_SIMPLEERRORRESPONSE_H
#include "Response.h"
#include "../../exceptions/HttpException.h"

/**
 * Response to a HTTP/0.9 request that creates an exception
 *
 * Sends to the client the message of the exception
 */
class SimpleErrorResponse: public Response {
protected:
    const std::string what;
public:
    SimpleErrorResponse(HttpConnection &connection, const HttpException &exception);
    /**
    *
    * @return boolean true on success, false on failure
    */
    bool send() override;

    virtual std::string getFullMessage();
    std::string getPartialMessage() override;

};



#endif //HTTP_SERVER_SIMPLEERRORRESPONSE_H
