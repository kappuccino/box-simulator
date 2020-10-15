#include <iostream>

#include "Serial.h"

const char *port = R"(\\.\COM3)";

int main() {
    const int DATA_LENGHT = 1;
    Serial *serialConn;
    std::cout << "Connexion..." << std::endl;
    serialConn = new Serial(port);

    if (serialConn->isConnected()){
        while(true) {
            char received;
            bool hasRead = serialConn->readChar(&received);
            if (hasRead) {
                std::cout << "RX: " << received << "\n";
            }else {
                std::cerr << "ERROR reading data\n";
                break;
            }
        }
    }
    return 0;
}
