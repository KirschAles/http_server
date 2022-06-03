#ifndef HTTP_SERVER_SERVER_H
#define HTTP_SERVER_SERVER_H
#include "../configuration.h"
#include "Connection.h"

class Server {
    int sockfd;
    Configuration configuration;
    addrinfo *addressInfo;
    int maxBacklog = 10;


    static void setAddrInfo(struct addrinfo& info);
public:
    Server(Configuration configuration=Configuration());
    // binds socket to a port (and address)
    // RETURN VALUE: true on success, false on failure
    bool bind();

    // listens for incoming connections
    // RETURN VALUE: true on succes, false on failure
    bool listen();

    // Accepts incoming connection
    // RETURN VALUE: new connection
    // ERRORS: std::runtime_error if the connection couldn't be accepted
    Connection accept();
    ~Server();
};


#endif //HTTP_SERVER_SERVER_H
