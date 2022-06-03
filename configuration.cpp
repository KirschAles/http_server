#include "configuration.h"

// loads file
bool Configuration::load(const std::string &configFile) {
    return true;
}

template<class T>
bool Configuration::isIn(const T& item, const std::vector<T> &items) {
    for (auto &thing: items) {
        if (item == thing) {
            return true;
        }
    }
    return false;
}