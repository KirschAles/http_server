#ifndef HTTP_SERVER_LOGGER_H
#define HTTP_SERVER_LOGGER_H
#include <fstream>
#include "configuration.h"
#include <ctime>

class Logger {
private:
    std::ofstream file;
    const Configuration &configuration;
    std::string format;

    bool isLogFormatValid() const;
public:
    Logger(const Configuration &configuration);
    void log(const std::string &message);

};


#endif //HTTP_SERVER_LOGGER_H
