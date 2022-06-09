#ifndef HTTP_SERVER_SERVER_H
#define HTTP_SERVER_SERVER_H
#include "../configuration.h"
#include "Connection.h"
#include <fcntl.h>

/**
 * Server
 *
 * Sets up a socket, to listen on inputted port and creates Connections if any are accepted
 */
class Server {
    int sockfd;
    Configuration configuration;
    addrinfo *addressInfo;
    int maxBacklog = 10;


    static void setAddrInfo(struct addrinfo& info);
public:
    Server(Configuration configuration=Configuration());
    bool bind();
    bool listen();
    Connection accept();
    ~Server();
};


#endif //HTTP_SERVER_SERVER_H
