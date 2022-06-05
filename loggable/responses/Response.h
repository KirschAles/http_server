#ifndef HTTP_SERVER_RESPONSE_H
#define HTTP_SERVER_RESPONSE_H
#include "../Loggable.h"
#include "../../contentGenerator.h"
class Response: Loggable {
public:
    virtual bool send() = 0;

};


#endif //HTTP_SERVER_RESPONSE_H
