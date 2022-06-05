#ifndef HTTP_SERVER_RESPONSE_H
#define HTTP_SERVER_RESPONSE_H
#include "../Loggable.h"
#include "../../ContentGenerator.h"
class Response: public Loggable {
public:
    virtual bool send() = 0;

};


#endif //HTTP_SERVER_RESPONSE_H
