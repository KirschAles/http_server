#include "Connection.h"
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
    do {
        int wantedLength = (bytesWanted>BUFFER-1) ? BUFFER-1 : bytesWanted;

        bytesRecieved = recv(sockfd, (void *)buffer, wantedLength, 0);
        // loop is used instead of normal addition,
        // so an user can send \0 in data
        // which if he sends binary data is a possibility
        // in the event of error, bytes recieved is -1 and so no loop show take place
        for (int i = 0; i < bytesRecieved; i++) {
            message += buffer[i];
        }
        // dont decrement bytesWanted, if the max size is set to unlimited (0)
        // this is ok, because bytesWanted won't be ever used, if the bytesRecieved is -1
        bytesWanted -= maximumSize?bytesRecieved:0;
    } while(bytesWanted != 0 && -1 != bytesRecieved);
    // run the loop until all wanted bytes are sent or the client stops sending
    delete[] buffer;
    return std::move(message);
}
Connection::~Connection(){
    close();
}
