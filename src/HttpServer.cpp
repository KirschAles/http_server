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
    Logger *logger = nullptr;
    try {
        logger = new Logger(configuration);
    }
    catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        if (logger) {
            delete logger;
        }
        return -1;
    }
    while (keepRunning) {
        Connection connection = server.accept();
        std::cout << "Accepted" << std::endl;
        HttpConnection conn(connection, configuration);
        Communication comm(conn, configuration);
        keepRunning = comm.communicate(*logger);
        std::cout << "Closed" << std::endl;
    }
    delete logger;
    return 0;
}



/*! \mainpage HttpServer
 *
 * \section intro_sec Introduction
 *
 * HttpServer serves the purpose of a http server. It recieves http requsts and responds with http/0.9 or http/1.0 responses.
 * The programm is run ./HttpServer.exe <optionalConfig> with an optional argument of the name of configuration file.
 * If argument is inputted, server will choose the default option, config.txt as it's configuration file from which it will
 * load configurations.
 * \section Configurations
 *          "port"
 *          "IP-address"
 *          "root-directory"
 *          "bytes-at-time"
 *          "log-file"
 *          "shutdown"
 *          "script-extensions"
 *          "text-extensions"
 *          "log-format"
 *          "timout-micro-seconds"
 *          "timeout-seconds"
 *  The configuration name must be immedietly followed by ':', spacebar and value.
 *  extensions can have multiple values, those should be written behind each other and separeted by ',' and spacebar.
 *  If bad value is inserted. Program will shutdown. If no value for somePart is inserted default value will be used.
 *  If something else is in the config file except for headers or empty lines. Programm will shutdown.
 *
 * \section install_sec ServerLoop
 *  Programm than initializes server, which is constructed, binded to ipAddress and port and than set to listen.
 *  After that Logger is initialized. If logger cannot be opened, programm shuts down.
 *  Than the main loop is entered. Where Connections are constantly accepted, until shutdown file is requested.
 */