#include <iostream>

#include "Serial.h"

const char *port = R"(\\.\COM3)";

int main() {
    const int DATA_LENGHT = 1;
    Serial *serialConn;
    std::cout << "Connexion...";
    serialConn = new Serial(port);

    if (serialConn->isConnected()) {
        std::cout << "OK" << std::endl;
        //char received;
        std::string cmd;

        while (true) {
            cmd.clear();
            bool hasRead = serialConn->readCommand(&cmd);
            if (hasRead) {
                std::cout << '"' << cmd << '"' << std::endl;
            } else {
                std::cerr << "ERROR reading data\n";
                break;
            }
        }
    }
    return 0;
}
