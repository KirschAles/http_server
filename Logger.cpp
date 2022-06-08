#include "Logger.h"
#include "constants/format.h"

bool isFormatString(const std::string &formatString) {
    if (formatString.length() != 2) {
        return false;
    }
    for (auto &format: format::allFormats) {
        if (formatString == format) {
            return true;
        }
    }
    return false;
}
bool Logger::isLogFormatValid() const {
    /* %% = %
     * %t = time
     * %m = message
     * %I = IP address
     * %p = port
     * %d = domain
     */
    if (format.length() == 0) {return true;}
    if (format.length() == 1 && format != format::formatter) {return true;}
    if (format.length() == 1) {return false;}

    for (size_t i = 0; i < format.length() - 1; i++) {
        if (format[i] == format::formatter[0]) {
            if (!isFormatString(format.substr(i,format::message.length()))) {
                return false;
            }
        }
    }



}

Logger::Logger(const Configuration &configuration)
: configuration(configuration), format(configuration.getLogFormat()){
    file.open(configuration.getLogFile(), std::ios_base::app);
    if (file.fail()) {
        throw std::runtime_error("File couldn't be opened.");
    }
    if (!isLogFormatValid()) {
        throw std::runtime_error("Format is invalid.")
    }
}
void Logger::log(const std::string &message) {

}