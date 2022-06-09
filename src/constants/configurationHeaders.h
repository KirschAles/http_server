#ifndef HTTP_SERVER_CONFIGURATIONHEADERS_H
#define HTTP_SERVER_CONFIGURATIONHEADERS_H
#include <string>
namespace configHeaders {
    const std::string port = "port";
    const std::string ipAddress = "IP-address";
    const std::string rootDirectory = "root-directory";
    const std::string chunkSize = "bytes-at-time";
    const std::string logFile = "log-file";
    const std::string killFile = "shutdown";
    const std::string scriptExtensions = "script-extensions";
    const std::string textExtensions = "text-extensions";
    const std::string logFormat = "log-format";
    const std::string timoutMicroSeconds = "timout-micro-seconds";
    const std::string timoutSeconds = "timeout-seconds";
}
#endif //HTTP_SERVER_CONFIGURATIONHEADERS_H
