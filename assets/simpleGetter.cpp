#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>

int PORT = 8080;
const std::string CRLF = "\r\n";
const std::string LF="\n\n";
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
    if (-1 == connect(sockfd, result->ai_addr, result->ai_addrlen)) {
        std::cout << "fail" << std::endl;
        return -1;
    }
    while (true) {

        std::cout << "Connected" << std::endl;

        std::string message;
        getline(std::cin, message);
        std::cout << message;
        message += CRLF + CRLF;
        int originalLength = message.length();
        int length = message.length();
        int bytesSent = 0;

        while (length -= bytesSent) {
            bytesSent = send(sockfd, (void *) (message.c_str() + originalLength - length), length, 0);
        }
        char recieved[100];
        recieved[recv(sockfd, (void *) recieved, 100, 0)] = '\0';
        std::cout << "recieved: " << recieved << std::endl;
    }
    close(sockfd);
};
