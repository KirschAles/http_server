#include "Connection.h"
Connection::Connection(int sockfd, sockaddr_storage &addr, size_t addrSize, const Configuration &configuration)
        :sockfd(sockfd), configuration(configuration) {
    memcpy(&connectedAddr, &addr, addrSize);
}
Connection::Connection(const Connection &connection)
        :sockfd(connection.sockfd), configuration(connection.configuration) {
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

bool Connection::send(const std::string &message) const noexcept{
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
std::string Connection::recieve() const {
    return recieve(0);
}
// INPUT: maximum size of the message, 0 if not limit on the size
// RETURNS: message recieved
// EFFECT: recieves messege from the connection
// ERRORS: throws runtime_error if the recieving of message fails
std::string Connection::recieve(size_t maximumSize) const {
    char *buffer = new char[BUFFER];
    size_t bytesWanted = maximumSize?maximumSize:BUFFER-1;
    int bytesRecieved = 0;
    std::string message{};

    fd_set setOfSockets;
    FD_ZERO(&setOfSockets); // clear the structure
    FD_SET(sockfd, &setOfSockets); // add our socket to the structure

    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    do {

        int canWeRecievie = select(sockfd+1, &setOfSockets, NULL, NULL, &timeout);
        if (!(canWeRecievie > 0) ) {
            // nothing to read
            // connection manager will deal with the fact that the input may be empty
            break;
        }

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
    return std::move(message);
}
Connection::~Connection(){
    close();
}
