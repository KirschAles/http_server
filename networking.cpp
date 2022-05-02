#include "networking.h"
Connection::Connection(int sockfd, sockaddr_storage &addr, size_t addrSize)
:sockfd(sockfd) {
    memcpy(&connectedAddr, &addr, addrSize);
}
Connection::Connection(const Connection &connection)
:sockfd(connection.sockfd) {
    memcpy(&connectedAddr, &connection.connectedAddr, sizeof connectedAddr);
}
Connection &Connection::operator=(const Connection &connection) {
    sockfd = connection.sockfd;
    memcpy(&connectedAddr, &connection.connectedAddr, sizeof connectedAddr);
    return *this;
}

    // PARAMETERS: STRING message to be send
    // RETURNS: true if the whole message was sent, false otherwise
    // EFFECTS: sends the message to the other side of a connection

bool Connection::send(const std::string &message) noexcept {
    size_t bytesSent = 0;
    size_t current_index = 0;
    size_t bytesToSend = message.length();
    const char *msg = message.c_str();

    while ((bytesSent = ::send(sockfd, &msg[current_index], bytesToSend, 0))) {
        current_index += bytesSent;
        bytesToSend -= bytesSent;
    }
    return true;
}
    // INPUT: None
    // RETURNS: message recieved
    // EFFECT: recieves messege from the connection
    // ERRORS: throws runtime_error if the recieving of message fails
std::string Connection::recieve() {
    return recieve(0);
}
    // INPUT: maximum size of the message, 0 if not limit on the size
    // RETURNS: message recieved
    // EFFECT: recieves messege from the connection
    // ERRORS: throws runtime_error if the recieving of message fails
std::string Connection::recieve(size_t maximumSize) {
        char *buffer = new char[BUFFER];
        size_t bytesWanted = maximumSize?maximumSize:BUFFER-1;
        int bytesRecieved = 0;
        std::string message{};
        do {
            int wantedLength = (bytesWanted>BUFFER-1) ? BUFFER-1 : bytesWanted;

            bytesRecieved = recv(sockfd, (void *)buffer, wantedLength, 0);
            if (bytesRecieved == -1) {
                delete[] buffer;
                throw std::runtime_error("Message couldn't be accepted");
            }
            // loop is used instead of normal addition,
            // so an user can send \0 in data
            // which if he sends binary data is a possibility
            for (int i = 0; i < bytesRecieved; i++) {
                message += buffer[i];
            }
            // dont decrement bytesWanted, if the max size is set to unlimited (0)
            bytesWanted -= maximumSize?bytesRecieved:0;
        } while(bytesWanted != 0 && bytesRecieved == BUFFER-1);
        // run the loop until all wanted bytes are sent or the client stops sending
        delete[] buffer;
        return move(message);
}
Connection::~Connection(){
    close(sockfd);
}
void Server::setAddrInfo(struct addrinfo& info) {
    info.ai_family = AF_UNSPEC;
    info.ai_socktype = SOCK_STREAM;
    info.ai_flags = AI_PASSIVE;
}

Server::Server(Configuration config)
:config(config) {
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof hints);
    setAddrInfo(hints);    // fill in my IP for me
    if (getaddrinfo(NULL, config.getPort(), &hints, &result)) {
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
    return Connection(newSock, connectingAddr, addrSize);
}
Server::~Server(){
    freeaddrinfo(addressInfo);
}
