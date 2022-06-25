#ifndef HTTP_SERVER_HTTPCONNECTION_H
#define HTTP_SERVER_HTTPCONNECTION_H
#include "Connection.h"
#include <deque>

/**
 * HttpConnection
 *
 * Class made as container for a connection, so it is easier to deal with
 * has several function to better read HttpRequests
 */
class HttpConnection {
private:
    const Connection &connection;
    std::deque<char> buffer;
    size_t chunkLength;
    std::string recording;
    bool isRecording = false;
public:
    HttpConnection(const Connection &connection, const Configuration &configuration);
    void rollBackByte(char byte);
    bool newChunk();
    bool assertSize(size_t size);
    // checks if there is a CSLR at the start, on succes deletes the CSLR from the start
    // else return false and keeps it in the previous state
    bool isCSLR();
    char getByte();
    std::string getBytes(int length) ;
    std::string getLine() ;
    bool send(const std::string &message);
    void startRecording();
    void stopRecording();
    const std::string &getRecords();
    void clearRecords();
    std::string getIpAddress() const;
    std::string getDomain() const;

};


#endif //HTTP_SERVER_HTTPCONNECTION_H
