//
// Created by Toorop on 15/10/2020.
//

#ifndef FSBRIDGE_SERIAL_H
#define FSBRIDGE_SERIAL_H

#include <windows.h>
#include <iostream>

class Serial {

private:

    HANDLE handler;
    bool connected;
    COMSTAT status;
    DWORD errors;

public:
    explicit Serial(const char *portName);
    ~Serial();

    // read from serial
    int read(const char *buffer, unsigned int buf_size);
    // read one char
    bool readChar(char *c);
    // read command
    bool readCommand(std::string *cmd);
    // write to serial
    bool write(const char *buffer, unsigned int buf_size);
    // connected ??
    bool isConnected();
    // close serail com
    void close();
};


#endif //FSBRIDGE_SERIAL_H
