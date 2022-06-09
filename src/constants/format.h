#ifndef HTTP_SERVER_FORMAT_H
#define HTTP_SERVER_FORMAT_H
#include <string>
#include <vector>
namespace format {
    const std::string formatter = "%";
    const std::string message = formatter + "m";
    const std::string time = formatter + "t";
    const std::string ipAddress = formatter + "I";
    const std::string domain = formatter + "d";
    const std::string procent = formatter + formatter;

    std::vector<std::string> allFormats {message, time, ipAddress, domain, procent};
}

#endif //HTTP_SERVER_FORMAT_H
