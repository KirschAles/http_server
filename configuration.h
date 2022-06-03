#ifndef HTTP_SERVER_CONFIGURATION_H
#define HTTP_SERVER_CONFIGURATION_H
#include <iostream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;
class Configuration {
private:
    std::string port = "8080";
    char *ipAddress = NULL;
    fs::path rootDirectory = fs::canonical(fs::path("."));
    int chunkSize = 1000;
    fs::path logFile = fs::canonical(fs::path("./log.log"));
public:
    Configuration(){}
    bool load(const std::string file);

    // returns pointer to the start of the port string
    // needed for the functions from the socket library
    const char *getPort() const {return port.c_str();}
    const char *getIP() const {return ipAddress;}
    const fs::path &getRootDirectory() const {return rootDirectory;}
    int getChunkSize() const {return chunkSize;}
    const fs::path &getLog() const {return logFile;}
};

#endif //HTTP_SERVER_CONFIGURATION_H
