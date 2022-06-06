#ifndef HTTP_SERVER_MULTIPLECHOICES_H
#define HTTP_SERVER_MULTIPLECHOICES_H
#include <stdexcept>

class MultipleChoices: public std::runtime_error {
public:
    MultipleChoices(const std::string &eMessage): std::runtime_error(eMessage) {}
    MultipleChoices(const MultipleChoices &error): std::runtime_error(error) {}
    MultipleChoices(MultipleChoices &&error): std::runtime_error(std::move(error)) {}
    MultipleChoices &operator=(const MultipleChoices &error) = default;
};
#endif //HTTP_SERVER_MULTIPLECHOICES_H
