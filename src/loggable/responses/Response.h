#ifndef HTTP_SERVER_RESPONSE_H
#define HTTP_SERVER_RESPONSE_H
#include "../Loggable.h"
#include "../../communicationManagement/HttpConnection.h"

/**
 * Abstract class serving as a base for all responses
 */
class Response: public Loggable {
protected:
    HttpConnection &connection;
    Response(HttpConnection &connection): connection(connection){}
public:
    virtual bool send() = 0;
    virtual ~Response() = default;
};


#endif //HTTP_SERVER_RESPONSE_H
