#ifndef HTTP_SERVER_CONFIGURATION_H
#define HTTP_SERVER_CONFIGURATION_H
#include <iostream>
#include <experimental/filesystem>
#include <fstream>
#include <ctime>
#include "constants/configurationHeaders.h"
#include <map>
#include <cstring>
#include "constants/mixed.h"
#include <limits>

namespace fs = std::experimental::filesystem;
class Configuration {
private:
    std::string port = "8080";
    // needs to be char* so nullptr can be send
    char *ipAddress = nullptr;
    fs::path rootDirectory = fs::canonical(fs::path("."));
    size_t chunkSize = 100000;
    fs::path logFile = fs::path("./log.log");
    fs::path killFile = fs::path("./kill");
    std::vector<std::string> scriptExtensions = {".sh", ".py", ".exe", ".out"};
    std::vector<std::string> textExtensions = {".txt", ".cpp", ".h", ".html", ".xml", ".css", ".json", ".jss"};
    std::vector<std::string> imageExtensions = {".gif", ".jpeg", ".png", ".tiff", ".vnd.microsoft.icon", ".x-icon", ".vnd.djvu", ".svg+xml"};
    std::string logFormat = "log:\n \tdomain: %d \tIP address: %I\n\tmessage: %m\n\n";
    __suseconds_t timeoutMicroSeconds = 0;
    __time_t timeoutSeconds = 3;

    static bool isIn(const std::string &item, const std::vector<std::string> &items);
    void get(const std::string &configurationFile);
    static std::string getHeaderName(std::string &line);
    static void getHeader(std::string &line, std::map<std::string, std::string> &headers);
    static bool getHeaders(std::ifstream &input, std::map<std::string, std::string> &headers);
    static std::string getLine(std::ifstream &input, bool &canStillRead);
    static bool isOnlyWhiteSpace(const std::string &line);
    void setLoadedConfigurations(const std::map<std::string, std::string> &headers);
    static bool isNegative(const std::string &value);
    static std::string stripBackWhiteSpace(const std::string &value);
    static std::string stripFrontWhiteSpace(const std::string &value);


    void setPort(const std::string &);
    void setIP(const std::string &);
    void setRootDirectory(const std::string &);
    void setChunkSize(const std::string &);
    void setLogFile(const std::string &);
    void setScriptExtensions(const std::string &);
    void setTextExtensions(const std::string &);
    void setKillFile(const std::string &);
    void setLogFormat(const std::string &);
    void setTimeoutMicroSeconds(const std::string &);
    void setTimeoutSeconds(const std::string &);
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
    bool isScript(const std::string &extension) const {return isIn(extension, scriptExtensions);}
    bool isText(const std::string &extension) const {return isIn(extension, textExtensions);}
    bool isImage(const std::string &extension) const {return isIn(extension, imageExtensions);}
    const fs::path &getKillFile() const {return killFile;}
    const std::string &getLogFormat() const {return logFormat;}
    __suseconds_t getTimeoutMicroSeconds() const {return timeoutMicroSeconds;}
    __time_t getTimeoutSeconds() const {return timeoutSeconds;}

    ~Configuration();

};

#endif //HTTP_SERVER_CONFIGURATION_H
