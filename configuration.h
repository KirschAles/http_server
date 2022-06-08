#ifndef HTTP_SERVER_CONFIGURATION_H
#define HTTP_SERVER_CONFIGURATION_H
#include <iostream>
#include <experimental/filesystem>
#include <fstream>
#include <ctime>

namespace fs = std::experimental::filesystem;
class Configuration {
private:
    std::string port = "8080";
    char *ipAddress = NULL;
    fs::path rootDirectory = fs::canonical(fs::path("."));
    size_t chunkSize = 100000;
    fs::path logFile = fs::path("./log.log");
    fs::path killFile = fs::path("./kill");
    std::vector<std::string> scriptExtensions = {".sh", ".py", ".exe", ".out"};
    std::vector<std::string> textExtensions = {".txt", ".cpp", ".h", ".html", ".xml", ".css", ".json"};
    std::string logFormat = "log:\n \tdomain: %d \tIP address: %I\n\tmessage: %m\n\n";
    __suseconds_t timoutMicroSeconds = 0;
    __time_t timoutSeconds = 3;

    static bool isIn(const std::string &item, const std::vector<std::string> &items);
public:
    Configuration() = default;
    bool load(const std::string &file);

    // returns pointer to the start of the port string
    // needed for the functions from the socket library
    const char *getPort() const {return port.c_str();}
    const char *getIP() const {return ipAddress;}
    const fs::path &getRootDirectory() const {return rootDirectory;}
    size_t getChunkSize() const {return chunkSize;}
    const fs::path &getLogFile() const {return logFile;}
    bool isScript(const std::string &extension) const {return isIn(extension, scriptExtensions);};
    bool isText(const std::string &extension) const {return isIn(extension, textExtensions);};
    const fs::path &getKillFile() const {return killFile;}
    const std::string &getLogFormat() const {return logFormat;}
    __suseconds_t getTimeoutMicroSeconds() const {return timoutMicroSeconds;}
    __time_t getTimeoutSeconds() const {return timoutSeconds;}
};

#endif //HTTP_SERVER_CONFIGURATION_H
