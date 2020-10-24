//
// Created by Toorop on 15/10/2020.
//

#include "Serial.h"

// constructor
Serial::Serial(const char *portName) {
    this->connected = false;
    this->handler = CreateFileA(static_cast<LPCSTR>(portName),
                                GENERIC_READ | GENERIC_WRITE,
                                0,
                                NULL,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);

    if (this->handler == INVALID_HANDLE_VALUE) {
        if(GetLastError() == ERROR_FILE_NOT_FOUND){
            std::cerr << "ERROR: " << portName << " is not available\n";
        } else {
            std::cerr << "ERROR: " << GetLastError() << "\n";
        }
    // OK
    } else {
        DCB dcbSerialParameters = {0};
        if (!GetCommState(this->handler, &dcbSerialParameters)){
            std::cerr << "Failed to get current serial parameters\n";
        } else {
            dcbSerialParameters.BaudRate = CBR_115200;
            dcbSerialParameters.ByteSize = 8;
            dcbSerialParameters.StopBits = ONESTOPBIT;
            dcbSerialParameters.Parity = NOPARITY;
            dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

            if (!SetCommState(this->handler, &dcbSerialParameters)){
                std::cout << "ERROR: could not set serial parameters \n";
            } else {
                this->connected = true;
                PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
                Sleep(2000);
            }
        }
    }
}

// destructor
Serial::~Serial() {
    if (this->connected){
        CloseHandle(this->handler);
        this->connected = false;
    }
}

// read from serial port
// retoune 0 en cas d'erreur
int Serial::read(const char *buffer, unsigned int buf_size) {
    DWORD bytesRead{};
    unsigned int toRead {0};
    ClearCommError(this->handler, &this->errors, &this->status);

    if (this->status.cbInQue > buf_size){
        toRead = buf_size;
    } else  {
        toRead = this->status.cbInQue;
    }

    memset((void*) buffer, 0, buf_size);

    if (ReadFile(this->handler, (void*) buffer, toRead, &bytesRead, NULL)){
        return bytesRead;
    }
    return 0;
}

bool Serial::readChar(char *c) {
   // DWORD bytesRead{};
    // ClearCommError(this->handler, &this->errors, &this->status);
    //memset((void*) c, 0, 1);
    //*c=0;
    while(true) {
        if (ReadFile(this->handler, (void *) c, 1, NULL, NULL)) {
            //printf("%d\n",*c);
            if (*c != 0) return true;
        }
    }
    return false;
}

// read command
// each command is separated by byte 3
bool Serial::readCommand(std::string *cmd) {
    char c;
    while(true){
        if (this->readChar(&c)){
            if (c == 3 && cmd->length() > 0){
                return true;
            }       
            if (c >= 0) {
                cmd->push_back(c);
            }
        } else {
            break;
        }
    }
    return false;
}


// write to serial
// todo return bytesSent
bool Serial::write(const char *buffer, unsigned int buf_size) {
    DWORD bytesSend;

    if(!WriteFile(this->handler, (void*) buffer, buf_size, &bytesSend,0)){
        ClearCommError(this->handler, &this->errors, &this->status);
        return false;
    }
    return true;
}


// serial is connected ?
bool Serial::isConnected() {
    if (!ClearCommError(this->handler, &this->errors, &this->status)){
        this->connected = false;
    }
    return this->connected;
}

// close
void Serial::close() {
    CloseHandle(this->handler);
}




