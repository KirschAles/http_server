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
    return true;


}

Logger::Logger(const Configuration &configuration)
: configuration(configuration), format(configuration.getLogFormat()){
    file.open(configuration.getLogFile(), std::ios_base::app);
    if (file.fail()) {
        throw std::runtime_error("File couldn't be opened.");
    }
    if (!isLogFormatValid()) {
        throw std::runtime_error("Format is invalid.");
    }
}
void Logger::log(const std::string &message, std::string ipAddress, std::string domain) {
    for (size_t i = 0; i < format.length(); i++) {
        if (!isFormatString(format.substr(i, format::message.length()))) {
            file << format[i];
        }
        else {
            std::string output;
            std::string substring = format.substr(i, format::message.length());
            if (substring == format::message) {output = message;}
            else if (substring == format::ipAddress) {output = ipAddress;}
            else if (substring == format::domain) {output = domain;}
            else if (substring == format::time) {
                std::time_t t = std::time(nullptr);
                size_t length = configuration.getChunkSize();
                char *tmp = new char[length];
                if (std::strftime(tmp, sizeof(*tmp) *length, "%c", std::localtime(&t))) {
                    output = std::string(tmp);
                }
                else {
                    output = mixed::unknown;
                }
                delete [] tmp;
            }
            // this only leaves out format::formatter+format::formatter
            else  {output = format::formatter;}
            file << output;
            // substring has two character, so we need to increment
            i++;
        }
    }
}