#include "configuration.h"

// loads file
std::string Configuration::getHeaderName(std::string &line) {
    std::string name;
    char tmp;
    size_t pos;
    for (pos = 0; ':' != (tmp = line[pos]); pos++) {
        // attribute name must be made of printable characters
        if (!std::isprint(tmp)) {
            throw std::runtime_error("Unprintable character in configuratin header.");
        }
        else if (std::isspace(tmp)) {
            throw std::runtime_error("Configuration name in request contains whitespace characters");
        }
        // the header name must not depend on the case of character
        name += tolower(tmp);
        if (pos == line.length() - 1) {
            throw std::runtime_error("No ':' character in header");
        }
    }
    if (pos >= line.length() - 2) {
        throw std::runtime_error("No value in header");
    }
    if (line[++pos] != ' ') {
        throw std::runtime_error("Bad format of header, missing space after ':'");
    }
    line = line.substr(++pos);
    return name;
}
void Configuration::getHeader(std::string &line, std::map<std::string, std::string> &headers) {
    std::string name = getHeaderName(line);
    std::cout << name << std::endl;
    std::cout << line << std::endl;
    headers[name] += stripFrontWhiteSpace(stripBackWhiteSpace(line));
    std::cout << headers.at(name) << std::endl;
}
std::string Configuration::getLine(std::ifstream &input, bool &canStillRead) {
    std::string line;
    while (true) {
        char c = input.get();
        if (input.eof()) {
            canStillRead = false;
            return line;
        }
        if (c == '\n') {
            return line;
        }
        line += c;
    }
}
bool Configuration::isOnlyWhiteSpace(const std::string &line) {
    for (char character: line) {
        if (!std::isspace(character)) {
            return false;
        }
    }
    return true;
}
bool Configuration::getHeaders(std::ifstream &input, std::map<std::string, std::string> &headers) {
    bool canStillRead = true;
    while (canStillRead) {
        std::string line = getLine(input, canStillRead);
        if (!line.empty() && !isOnlyWhiteSpace(line)) {
            getHeader(line, headers);
        }
    }
    return true;
}
void Configuration::setPort(const std::string &newValue) {
    if (isNegative(newValue)) {
        throw std::invalid_argument("Port cannot be negative.");
    }
    int test = std::stoi(newValue);
    uint16_t max = std::numeric_limits<uint16_t>::max();
    if (test > max) {
        throw std::invalid_argument("Port cannot be more than 2 bytes big.");
    }
    port = newValue;
}
void Configuration::setIP(const std::string &newValue) {
    if (ipAddress) {
        delete [] ipAddress;
        ipAddress = nullptr;
    }
    ipAddress = new char[newValue.length() + 1];
    std::strncpy(ipAddress, newValue.c_str(), newValue.length() + 1);
}
void Configuration::setRootDirectory(const std::string &newValue) {
    rootDirectory = fs::canonical(fs::path(newValue));
}
std::string Configuration::stripBackWhiteSpace(const std::string &value) {
    if (value.empty()) {return std::string();}
    size_t i;
    for ( i = value.length()-1; i != 0; i--) {
        if (!std::isspace(value[i])) {
            break;
        }
    }
    std::cout << i << "   " << value.substr(0, i+1) << std::endl;
    return value.substr(0, i+1);
}
std::string Configuration::stripFrontWhiteSpace(const std::string &value) {
    std::string line;
    size_t i = 0;
    for (;i < value.length(); i++) {
        if (!std::isspace(value[i])) {
            break;
        }
    }
    return value.substr(i);
}
bool Configuration::isNegative(const std::string &value) {
    return value[0] == '-';
}

void Configuration::setLogFile(const std::string &newValue) {
    logFile = fs::path(newValue);
}
std::vector<std::string> divideByDash(const std::string &newValue) {
    std::vector<std::string> divided;
    std::string part;
    for (auto character: newValue) {
        if (character == mixed::divider && !part.empty()) {
            divided.emplace_back(part);
        }
        else if (!std::isspace(character)) {
            part += character;
        }
        // ignores if the character is space
    }
    return divided;
}
void Configuration::setScriptExtensions(const std::string &newValue) {
    scriptExtensions = divideByDash(newValue);
}
void Configuration::setTextExtensions(const std::string &newValue) {
    textExtensions = divideByDash(newValue);
}
void Configuration::setKillFile(const std::string &newValue) {
    killFile = fs::path(newValue);
}
void Configuration::setLogFormat(const std::string &newValue) {
    logFormat = newValue;
}
void Configuration::setTimeoutMicroSeconds(const std::string &newValue) {
    if (isNegative(newValue)) {
        throw std::runtime_error("Timeout cannot be negative.");
    }
    timeoutMicroSeconds = std::stol(newValue);
}
void Configuration::setTimeoutSeconds(const std::string &newValue) {
    if (isNegative(newValue)) {
        throw std::runtime_error("Timeout cannot be negative.");
    }
    timeoutSeconds = std::stol(newValue);
}



void Configuration::setLoadedConfigurations(const std::map<std::string, std::string> &headers) {
    for (auto &config: headers) {
        if (config.first == configHeaders::port) {setPort(config.second);}
        else if(config.first == configHeaders::ipAddress) {setIP(config.second);}
        else if(config.first == configHeaders::rootDirectory) {setRootDirectory(config.second);}
        else if(config.first == configHeaders::logFile) {setLogFile(config.second);}
        else if(config.first == configHeaders::killFile) {setKillFile(config.second);}
        else if(config.first == configHeaders::scriptExtensions) {setScriptExtensions(config.second);}
        else if(config.first == configHeaders::textExtensions) {setTextExtensions(config.second);}
        else if(config.first == configHeaders::logFormat) {setLogFormat(config.second);}
        else if(config.first == configHeaders::timoutMicroSeconds) {setTimeoutMicroSeconds(config.second);}
        else if(config.first == configHeaders::timoutSeconds) {setTimeoutSeconds(config.second);}
    }

}
void Configuration::get(const std::string &configFile) {
    std::ifstream input;
    input.open(configFile);
    if (input.fail()) {
        input.close();
        throw std::runtime_error("File failed to open.");
    }
    std::map<std::string, std::string> headers;
    getHeaders(input, headers);
    setLoadedConfigurations(headers);
}
bool Configuration::load(const std::string &configFile) {
    try {
        get(configFile);
    }
    catch (std::invalid_argument e) {
        std::cout << "Invalid Configuration argument." << std::endl;
        return false;
    }
    catch (std::out_of_range e) {
        std::cout << "Numbers were too big." << std::endl;
        return false;
    }
    catch (std::runtime_error e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    catch (std::exception e) {
        std::cout << "Error occurred when loading configuration file." << std::endl;
        return false;
    }
    return true;
}

bool Configuration::isIn(const std::string &item, const std::vector<std::string> &items) {
    for (auto &thing: items) {
        if (item == thing) {
            return true;
        }
    }
    return false;
}

Configuration::~Configuration() {
    if (ipAddress) {
        ipAddress = nullptr;
        delete [] ipAddress;
    }
}