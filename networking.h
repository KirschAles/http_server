//
// Created by alesp on 26.04.2022.
//

#ifndef HTTP_SERVER_NETWORKING_H
#define HTTP_SERVER_NETWORKING_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include "configuration.h"

constexpr size_t BUFFER = 1000;
// Class represents an established connection
class Connection {
private:
    int sockfd;
    sockaddr_storage connectedAddr;
public:
    Connection(int sockfd, sockaddr_storage &addr, size_t addrSize);
    Connection(const Connection &connection);
    Connection &operator=(const Connection &connection);

    // PARAMETERS: STRING message to be send
    // RETURNS: true if the whole message was sent, false otherwise
    // EFFECTS: sends the message to the other side of a connection
    bool send(const std::string &message) noexcept;
    // INPUT: None
    // RETURNS: message recieved
    // EFFECT: recieves messege from the connection
    // ERRORS: throws runtime_error if the recieving of message fails
    std::string recieve();
    // INPUT: maximum size of the message, 0 if not limit on the size
    // RETURNS: message recieved
    // EFFECT: recieves messege from the connection
    // ERRORS: throws runtime_error if the recieving of message fails
    std::string recieve(size_t maximumSize);
    ~Connection();
};

class Server {
    int sockfd;
    Configuration config;
    addrinfo *addressInfo;
    int maxBacklog = 10;


    static void setAddrInfo(struct addrinfo& info);
public:
    Server(Configuration config=Configuration());
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
#endif //HTTP_SERVER_NETWORKING_H
