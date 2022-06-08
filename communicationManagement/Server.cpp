#include "Server.h"

void Server::setAddrInfo(struct addrinfo& info) {
    info.ai_family = AF_UNSPEC;
    info.ai_socktype = SOCK_STREAM;
    info.ai_flags = AI_PASSIVE;
}

Server::Server(Configuration configuration)
        :configuration(configuration) {
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof hints);
    setAddrInfo(hints);    // fill in my IP for me
    if (getaddrinfo(configuration.getIP(), configuration.getPort(), &hints, &result)) {
        throw std::exception();
    }
    sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    addressInfo = result;
}
// binds socket to a port (and address)
// RETURN VALUE: true on success, false on failure
bool Server::bind() {
    return 0==::bind(sockfd, addressInfo->ai_addr, addressInfo->ai_addrlen);
}

// listens for incoming connections
// RETURN VALUE: true on succes, false on failure
bool Server::listen() {
    return ::listen(sockfd, maxBacklog);
}

// Accepts incoming connection
// RETURN VALUE: new connection
// ERRORS: std::runtime_error if the connection couldn't be accepted
Connection Server::accept() {
    struct sockaddr_storage connectingAddr;
    socklen_t addrSize = sizeof(connectingAddr);
    int newSock = ::accept(sockfd, (struct sockaddr *)&connectingAddr, &addrSize);
    if (newSock == -1) {
        throw std::runtime_error("Connection couldn't be accepted.");
    }
    return Connection(newSock, connectingAddr, addrSize, configuration);
}
Server::~Server(){
    freeaddrinfo(addressInfo);
}