#ifndef HTTP_SERVER_LOGGABLE_H
#define HTTP_SERVER_LOGGABLE_H
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

class Loggable {
public:
    virtual void log(const fs::path &file) = 0;
};


#endif //HTTP_SERVER_LOGGABLE_H
