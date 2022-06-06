#include <iostream>
#include "configuration.h"
#include "communicationManagement/Server.h"
#include "communicationManagement/Communication.h"

const char * DEFAULT_CONFIG = "./config.txt";
/* TODO:
 *      implement turning off of the server
 *      implement actual error handling
 *      replace runtime_error() with custom exception_classes
 *      fix integer overflow vulnerability on filePasser::calculateSize()
 *      decide how to handle URIs with spaces in them
 *      implement Configuration::load()
 *      implement log() for both requests and responses
 *      implement a costumizable way to decide how should different fileExtensions be handled
 *      add timeout to connection while recieving message
 */


int main(int argc, char *argv[]) {
    Configuration configuration;
    /*if (argc >= 2) {
        configuration.load(argv[1]);
    }
    else {
        configuration.load(DEFAULT_CONFIG);
    }*/
    std::cout << "here";
    Server server(configuration);
    server.bind();
    server.listen();
    std::cout << "Listening" << std::endl;
    // TO DO: implemented a way to turn the server off
    while (true) {
        Connection connection = server.accept();
        std::cout << "Accepted" << std::endl;
        HttpConnection conn(connection);
        Communication comm(conn, configuration);
        comm.recieveRequest();
        comm.respond();
    }
    return 0;
}