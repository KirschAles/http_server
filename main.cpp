#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <cstring>
// Class represents an established connection
class Connection {
private:
    int sockfd;
    sockaddr_storage connectedAddr;
public:
    Connection(int sockfd, sockaddr_storage &addr, size_t addrSize)
    :sockfd(sockfd) {
        memcpy(&connectedAddr, &addr, addrSize);
    }
    Connection(const Connection &connection)
    :sockfd(connection.sockfd) {
        memcpy(&connectedAddr, &connection.connectedAddr, sizeof connectedAddr);
    }
    Connection &operator=(const Connection &connection) {
        sockfd = connection.sockfd;
        memcpy(&connectedAddr, &connection.connectedAddr, sizeof connectedAddr);
        return *this;
    }
};
class Configuration {
private:
    std::string port = "8080";
public:
    Configuration(){}
    bool load(std::istream file) {
        return true;
    }

    // returns pointer to the start of the port string
    // needed for the functions from the socket library
    const char *getPort() {
        return port.c_str();
    }
};
class Server {
    int sockfd;
    Configuration config;
    addrinfo *addressInfo;
    int maxBacklog = 10;

    static void setAddrInfo(struct addrinfo& info) {
        info.ai_family = AF_UNSPEC;
        info.ai_socktype = SOCK_STREAM;
        info.ai_flags = AI_PASSIVE;
    }
public:
    Server(Configuration config=Configuration())
    :config(config) {
        struct addrinfo hints, *result;
        memset(&hints, 0, sizeof hints);
        setAddrInfo(hints);    // fill in my IP for me
        if (getaddrinfo(NULL, config.getPort(), &hints, &result)) {
            std::cout << "getaddrinfo failed during Connection creation." << std::endl;
            throw std::exception();
        }
        sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        addressInfo = result;
    }
    // binds socket to a port (and address)
    // RETURN VALUE: true on success, false on failure
    bool bind() {
        return 0==::bind(sockfd, addressInfo->ai_addr, addressInfo->ai_addrlen);
    }

    // listens for incoming connections
    // RETURN VALUE: true on succes, false on failure
    bool listen() {
        return ::listen(sockfd, maxBacklog);
    }

    // Accepts incoming connection
    // RETURN VALUE: new connection
    // ERRORS: std::runtime_error if the connection couldn't be accepted
    Connection accept() {
        struct sockaddr_storage connectingAddr;
        socklen_t addrSize = sizeof(connectingAddr);
        int newSock = ::accept(sockfd, (struct sockaddr *)&connectingAddr, &addrSize);
        if (newSock == -1) {
            throw std::runtime_error("Connection couldn't be accepted.");
        }
        return Connection(newSock, connectingAddr, addrSize);
    }
    ~Server(){
        freeaddrinfo(addressInfo);
    }
};


int main() {


}