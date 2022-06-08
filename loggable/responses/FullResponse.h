#ifndef HTTP_SERVER_FULLRESPONSE_H
#define HTTP_SERVER_FULLRESPONSE_H
#include "SimpleResponse.h"
#include "../../constants/http.h"
#include "../../constants/codes.h"

// response to a http/1.0 request or later, will always use http/1.0 standard
class FullResponse: public SimpleResponse {
protected:
    // the same stuff is in Request class, should rework it later into a Base class
    std::string httpVersion = http::HTTP10;
    std::map<std::string, std::string> headers;

    const Configuration &configuration;

    bool sendStatusLine();
    bool sendHeaders();
    std::string buildStatusLine() const;
    std::string buildHeaders() const;

        public:
    FullResponse(HttpConnection &connection, ContentGenerator &contentGenerator, const Configuration &configuration)
            : SimpleResponse(connection, contentGenerator), configuration(configuration) {}
    bool send() override;
    std::string getPartialMessage() override;
    std::string getFullMessage() override;
};


#endif //HTTP_SERVER_FULLRESPONSE_H
