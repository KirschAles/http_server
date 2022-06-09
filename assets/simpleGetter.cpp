#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>

int PORT = 8080;
const std::string CRLF = "/r/n";
int main (){
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;    // fill in my IP for me
    if (getaddrinfo("localhost", "8080", &hints, &result)) {
        std::cout << "getaddrinfo failed during Connection creation." << std::endl;
        throw std::exception();
    }
    int sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    while (true) {
        if (-1 == connect(sockfd, result->ai_addr, result->ai_addrlen)) {
            std::cout << "fail" << std::endl;
            return -1;
        }
        std::cout << "Connected" << std::endl;

        std::string message;
        getline(std::cin, message);
        std::cout << message << std::endl;
        char *sending = new char[message.length() + 4];
        int i = 0;
        for (; i < message.length(); i++){
            sending[i] = message[i];
        }
        sending[++i] = CRLF[0];
        sending[++i] = CRLF[1];
        sending[++i] = CRLF[0];
        sending[++i] = CRLF[1];
        int originalLength = message.length() + 4;
        int length = message.length() + 4;
        int bytesSent = 0;
        std::cout << length << std::endl;
        while (length -= bytesSent) {
            bytesSent = send(sockfd, (void *) (sending + originalLength - length), length, 0);
            std::cout << bytesSent << "   " << length << std::endl;
        }
        char recieved[100];
        recieved[recv(sockfd, (void *) recieved, 100, 0)] = '\0';
        std::cout << "recieved: " << recieved << std::endl;
    }
    close(sockfd);
};
