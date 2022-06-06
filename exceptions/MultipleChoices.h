#ifndef HTTP_SERVER_MULTIPLECHOICES_H
#define HTTP_SERVER_MULTIPLECHOICES_H
#include "HttpException.h"

class MultipleChoices: public HttpException {
public:
    MultipleChoices(const std::string &eMessage): HttpException(eMessage, codes::MultipleChoices) {}
    MultipleChoices(const MultipleChoices &error): HttpException(error) {}
    MultipleChoices(MultipleChoices &&error): HttpException(std::move(error)) {}
};
#endif //HTTP_SERVER_MULTIPLECHOICES_H
