#include <sys/socket.h>
#include <iostream>
int main() {
    int sck = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << sck << std::endl;
    return 0;
}