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



#include "../Logger/logger.h"

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
    std::string get_fixed_length_size(std::string message) {
        std::stringstream stream;
        stream << std::setw(SIZE_BYTES) << std::setfill('0') << message.size();
        std::string size = stream.str();
        return size;
    }

    size_t get_size_from(std::string fixed_length_string) {
        fixed_length_string.erase(0, std::min(fixed_length_string.find_first_not_of('0'), fixed_length_string.size() - 1));
        size_t size;
        if (fixed_length_string.size() < 1) {
            return 0;
        }
        try {
            size = std::stol(fixed_length_string);
        }
        catch (const std::exception& e) {
            std::cerr << fixed_length_string << " " << e.what() << '\n';
        }

        return size;
    }

    void send_message(SOCKET socket, std::string message, Logger* logger) {
        std::string encapsulated_string = get_fixed_length_size(message) + message;
        size_t total_bytes_sent = 0;
        size_t bytes_sent = 0;
        while (total_bytes_sent < encapsulated_string.size()) {
            std::string message_left = encapsulated_string.substr(bytes_sent);
            bytes_sent = send(socket, message_left.c_str(), message_left.size(), 0);
            total_bytes_sent += bytes_sent;
            logger->add_network("SEND", encapsulated_string.substr(total_bytes_sent - bytes_sent, total_bytes_sent), "<RECIVER>");
            if (bytes_sent < 0) {
                logger->add_error("MESSAGE NOT SENT");
                return;
            }
        }

    }
    std::string get_message(SOCKET socket, size_t size) {
        std::string message = "";
        char* recv_buffer = new char[BUFFER_SIZE];
        size_t bytes_recived = 0;
        while (bytes_recived < size) {
            memset(recv_buffer, '\0', BUFFER_SIZE);
            size_t len = recv(socket, recv_buffer, size - bytes_recived, 0);
            if (len < 0) {
                throw Socket_Error_Exception();
            }
            if (len == 0) {
                throw Client_Down_Exception();
            }
            std::string recv_msg = std::string(recv_buffer);
            message += recv_msg;
            bytes_recived += len;
        }
        delete recv_buffer;
        return message;
    }
    std::string recv_message(SOCKET socket, Logger* logger) {
        std::string message = "";
        std::string recv_string = get_message(socket, SIZE_BYTES);
        size_t message_size = get_size_from(recv_string);
        if (message_size > 0) {
            logger->add_network("RECV", "SIZE = " + std::to_string(message_size), "<SENDER>");
            message = get_message(socket, message_size);
            logger->add_network("RECV", "BODY = " + message, "<SENDER>");
        }

        return message;
    }

}


#endif