#ifndef HTTP_SERVER_CONFIGURATION_H
#define HTTP_SERVER_CONFIGURATION_H
#include <iostream>

class Configuration {
private:
    std::string port = "8080";
public:
    Configuration(){}
    bool load(const std::istream &file);

    // returns pointer to the start of the port string
    // needed for the functions from the socket library
    const char *getPort() {return port.c_str();}
};

#endif //HTTP_SERVER_CONFIGURATION_H
