#ifndef HTTP_SERVER_HTTP_H
#define HTTP_SERVER_HTTP_H

#include "networking.h"
#include "contentGenerator.h"
#include <deque>
#include <map>
#include "codes.h"


namespace http {
    constexpr char * HTTP09 = "HTTP/0.9";
    constexpr char * HTTP10 = "HTTP/1.0";

    // class for http Connection
    class Connection {
    private:
        const ::Connection &connection;
        std::deque<char> buffer;
        size_t chunkLength = 500;
    public:
        Connection(const ::Connection &connection)
                : connection(connection) {}
        void rollBackByte(char byte);
        bool newChunk();
        bool assertSize(size_t size);
        // checks if there is a CSLR at the start, on succes deletes the CSLR from the start
        // else return false and keeps it in the previous state
        bool isCSLR();
        char getByte();
        std::string getBytes(int length) ;
        std::string getLine() ;
        bool send(const std::string &message) {
            return connection.send(message);
        }

    };
    class Loggable {
    public:
        virtual void log(const fs::path &file) = 0;
    };

    class Request: Loggable {
    protected:
        std::string httpVersion;
        std::map<std::string, std::string> headers;
        std::string fileName;

        std::string setFileName(const std::string line);
        void setHttpVersion(const std::string &line) {
            // there should be nothing else but the http version here
            // if there is nothing than it means that the request is a simple request
            // and thus http/0.9
            httpVersion = line.length()>0 ? line : HTTP09;
        }
        void setRequestLine(Connection &connection);
        // line must be at least one character long, otherwise undefined behaviour
        std::string getHeaderName(std::string &line);
        void setHeader(std::string &line);
        bool setHeaders(Connection &connection);
    public:
        Request() {}
        // On ERROR throws std::runtime_error
        Request(Connection &connection);
        Request(const Request &request)
                : fileName(request.fileName), httpVersion(request.httpVersion), headers(request.headers) {}
        Request(Request &&request)
                : fileName(std::move(request.fileName)), httpVersion(request.httpVersion), headers(request.headers) {}
        Request &operator=(const Request &request);
        void log(const fs::path &logFile) override ;
        Request &operator=(Request &&request);
        const std::string &getFileName() {
            return fileName;
        }
        const std::map<std::string, std::string> &getHeaders() {
            return headers;
        }
        const std::string &getVersion() {
            return httpVersion;
        }
        void print();

    };
    class GETRequest: public Request {
    private:
    public:
        GETRequest(Connection &connection)
                : Request(connection) {
        }
    };
    // The class is only here because
    // using SimpleResponse as Base Response Class was too confusing
    class Response: public Loggable {
    public:
        void log(const fs::path &file) override;

        virtual bool send(contentGenerator &contentGenerator) = 0;
    };
    // response to a http/0.9 request
    class SimpleResponse: public Response {
    protected:
        Connection &connection;

        virtual bool sendBody(contentGenerator &contentGenerator);
    public:
        SimpleResponse(Connection &connection)
        : connection(connection) {}
        // dummy function only here so the real one can be used in fullResponse
        void log(const fs::path &file) override;
        bool send(contentGenerator &contentGenerator) override {
            return sendBody(contentGenerator);
        }
    };
    // response to a http/1.0 request or later, will always use http/1.0 standard
    class FullResponse: public SimpleResponse {
    protected:
        // the same stuff is in Request class, should rework it later into a Base class
        std::string httpVersion = HTTP10;
        std::map<std::string, std::string> headers;

        const Configuration &configuration;

        void sendStatusLine();
    public:
        FullResponse(Connection &connection, const Configuration &configuration)
        : SimpleResponse(connection), configuration(configuration) {}
        void send();
    };
    class FullResponseError: public FullResponse {};
    class Communication {
    private:
        Connection &connection;
        Request request;
        const Configuration &configuration;
        const std::string code = codes::OK;

    public:
        Communication(Connection &connection, const Configuration &configuration)
                : connection(connection), configuration(configuration) {}
        bool recieveRequest();
        void respond();
        void printRequest();
    };

}

#endif //HTTP_SERVER_HTTP_H
