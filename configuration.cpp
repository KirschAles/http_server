#include "configuration.h"

// loads file
bool Configuration::load(const std::string &configFile) {
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