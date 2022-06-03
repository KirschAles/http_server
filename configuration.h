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
    size_t chunkSize = 100000;
    fs::path logFile = fs::canonical(fs::path("./log.log"));
    std::vector<std::string> scriptExtensions = {"sh", "py", "exe", "out"};
    std::vector<std::string> textExtensions = {"txt", "cpp", "h", "html", "xml", "css", "json"};


    template<class T>
    static bool isIn(const T& item, const std::vector<T> &items);
public:
    Configuration() = default;
    bool load(const std::string &file);

    // returns pointer to the start of the port string
    // needed for the functions from the socket library
    const char *getPort() const {return port.c_str();}
    const char *getIP() const {return ipAddress;}
    const fs::path &getRootDirectory() const {return rootDirectory;}
    size_t getChunkSize() const {return chunkSize;}
    const fs::path &getLog() const {return logFile;}
    bool isScript(const std::string &extension) const {return isIn(extension, scriptExtensions);};
    bool isText(const std::string &extension) const {return isIn(extension, textExtensions);};

};

#endif //HTTP_SERVER_CONFIGURATION_H
