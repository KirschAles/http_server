#include <iostream>
#include "configuration.h"
#include "communicationManagement/Server.h"
#include "communicationManagement/Communication.h"
#include "Logger.h"
#include "constants/mixed.h"

/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    Configuration configuration;
    int loadedSuccesfully = false;
    if (argc >= 2) {
        loadedSuccesfully = configuration.load(argv[1]);
    }
    else {
        loadedSuccesfully = configuration.load(mixed::default_config);
    }
    if (!loadedSuccesfully) {
        std::cout << "Couldn't load configuration." << std::endl;
        return -1;
    }
    Server server(configuration);

    if (!server.bind() || !server.listen()) {
        std::cout << "Server couldn't be connected." << std::endl;
        return -1;
    }
    std::cout << "Listening..." << std::endl;
    bool keepRunning = true;
    Logger logger(configuration);
    while (keepRunning) {
        Connection connection = server.accept();
        std::cout << "Accepted" << std::endl;
        HttpConnection conn(connection, configuration);
        Communication comm(conn, configuration);
        keepRunning = comm.communicate(logger);
    }
    std::cout << "Turning off" << std::endl;
    return 0;
}