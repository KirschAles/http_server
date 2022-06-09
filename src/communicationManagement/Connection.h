#ifndef HTTP_SERVER_CONNECTION_H
#define HTTP_SERVER_CONNECTION_H


#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include "../configuration.h"
#include <arpa/inet.h>
#include "../constants/mixed.h"

/**
 * Connection
 *
 * Class made mostly as a container for C socket functions and structs that are needed for socket interaction
 */
class Connection {
private:
    int sockfd;
    sockaddr_storage connectedAddr;
    const Configuration &configuration;

    std::string getIp4Address() const;
    std::string getIp6Address() const;

    std::string getIp4Domain() const;
    std::string getIp6Domain() const;
public:
    Connection(int sockfd, sockaddr_storage &addr, size_t addrSize, const Configuration &configuration);
    Connection(const Connection &connection);
    Connection &operator=(const Connection &connection);

    // PARAMETERS: STRING message to be send
    // RETURNS: true if the whole message was sent, false otherwise
    // EFFECTS: sends the message to the other side of a connection
    bool send(const std::string &message) const noexcept;
    // INPUT: None
    // RETURNS: message recieved
    // EFFECT: recieves messege from the connection
    // ERRORS: throws runtime_error if the recieving of message fails
    std::string recieve() const;
    // INPUT: maximum size of the message, 0 if not limit on the size
    // RETURNS: message recieved
    // EFFECT: recieves messege from the connection
    // ERRORS: throws runtime_error if the recieving of message fails
    std::string recieve(size_t maximumSize) const;
    void close() {::close(sockfd);}
    std::string getIpAddress() const;
    std::string getDomain() const;
    ~Connection();
};


#endif //HTTP_SERVER_CONNECTION_H
