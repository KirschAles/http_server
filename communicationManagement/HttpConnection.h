#ifndef HTTP_SERVER_HTTPCONNECTION_H
#define HTTP_SERVER_HTTPCONNECTION_H
#include "Connection.h"
#include <deque>

// class for http Connection
class HttpConnection {
private:
    const Connection &connection;
    std::deque<char> buffer;
    size_t chunkLength = 500;
    std::string recording;
    bool isRecording = false;
public:
    HttpConnection(const Connection &connection)
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
    void startRecording() {isRecording = true;}
    void stopRecording() {isRecording = false;}
    const std::string &getRecords() {return recording;}
    void clearRecords() {recording = std::string();}
    std::string getIpAddress() const {return std::move(connection.getIpAddress());}
    std::string getDomain() const {return std::move(connection.getDomain());}

};


#endif //HTTP_SERVER_HTTPCONNECTION_H
