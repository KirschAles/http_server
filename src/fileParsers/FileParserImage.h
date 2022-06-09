#ifndef HTTP_SERVER_FILEPARSERIMAGE_H
#define HTTP_SERVER_FILEPARSERIMAGE_H
#include "FileParserBinary.h"
class FileParserImage: public FileParserBinary {
public:
    std::string getType() const override {return mixed::image;}
};

#endif //HTTP_SERVER_FILEPARSERIMAGE_H
