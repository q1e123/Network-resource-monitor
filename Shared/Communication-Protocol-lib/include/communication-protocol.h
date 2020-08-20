#ifndef COMMUNICATION_PROTOCOL_H
#define COMMUNICATION_PROTOCOL_H
#include <string>

#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
typedef int SOCKET;
#elif defined _WIN32 || defined _WIN64
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCKAPI_
#pragma comment( lib, "ws2_32.lib")
#include <winsock2.h>
#include <Ws2tcpip.h>
#endif

#include <algorithm>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <exception>

#include "logger.h"

#define BUFFER_SIZE 1024
#define SIZE_BYTES 10
class Socket_Error_Exception : public std::exception {
    virtual const char* what() const throw() {
        return "Unknown socket error";
    }
};

class Client_Down_Exception : public std::exception {
    virtual const char* what() const throw() {
        return "Client down";
    }
};

namespace Communication_Protocol {
    std::string get_fixed_length_size(std::string message);
    size_t get_size_from(std::string fixed_length_string);
    void send_message(SOCKET socket, std::string message, Logger* logger);
    std::string get_message(SOCKET socket, size_t size);
    std::string recv_message(SOCKET socket, Logger* logger);

}


#endif