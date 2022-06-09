#include <iostream>
#include "configuration.h"
#include "communicationManagement/Server.h"
#include "communicationManagement/Communication.h"
#include "Logger.h"
const char *DEFAULT_CONFIG = "./config.txt";
/* TODO:
 *      replace runtime_error() with custom exception_classes
 *      fix integer overflow vulnerability on filePasser::calculateSize()
 *      dealt with stuff in %n format
 *      implement Configuration::load()
 *      implement log() for both requests and responses
 *      implement a costumizable way to decide how should different fileExtensions be handled
 *      add timeout to connection while recieving message
 */


int main(int argc, char *argv[]) {
    Configuration configuration;
    int loadedSuccesfully = false;
    if (argc >= 2) {
        loadedSuccesfully = configuration.load(argv[1]);
    }
    else {
        loadedSuccesfully = configuration.load(DEFAULT_CONFIG);
    }
    if (!loadedSuccesfully) {
        std::cout << "Couldn't load configuration." << std::endl;
        return false;
    }
    Server server(configuration);

    if (!server.bind() || !server.listen()) {
        std::cout << "Server couldn't be connected." << std::endl;
        return -1;
    }
    std::cout << "Listening" << std::endl;
    // TO DO: implemented a way to turn the server off
    bool keepRunning = true;
    Logger logger(configuration);
    while (keepRunning) {
        Connection connection = server.accept();
        std::cout << "Accepted" << std::endl;
        HttpConnection conn(connection, configuration);
        Communication comm(conn, configuration);
        keepRunning = comm.communicate(logger);
    }
    std::cout << "turning off" << std::endl;
    return 0;
}