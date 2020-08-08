#ifndef COMMUNICATION_PROTOCOL_H
#define COMMUNICATION_PROTOCOL_H

#include <string>
#include <exception>
#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
typedef int SOCKET;
#elif defined _WIN32 || defined _WIN64
#pragma comment( lib, "ws2_32.lib")
#include <winsock2.h>
#include <Ws2tcpip.h>
#endif

#include "../Logger/logger.h"

#define BUFFER_SIZE 512

namespace Communication_Protocol{
    void send_message(SOCKET socket, std::string message, Logger *logger){
        size_t message_size = message.size();
        size_t left, right;
        left = 0;
        right = BUFFER_SIZE;

        send(socket, &message_size, sizeof(message_size), 0);
        logger->add_network("SEND", std::to_string(message_size), "<RECIVER>");

        while (left < message_size){
            std::string msg = message.substr(left, right);
            char *msg_c =  const_cast<char*>(msg.c_str());
            send(socket, msg_c, sizeof(msg_c), 0);
            logger->add_network("SEND", msg_c, "<RECIVER>");
            left = right;
            right += BUFFER_SIZE;
        } 
    }

    std::string recv_message(SOCKET socket, Logger *logger){
        std::string message = "";
        size_t left, right;
        left = 0;

        size_t message_size;
        recv(socket, &message_size, sizeof(message_size), 0);
        logger->add_network("RECV", std::to_string(message_size), "<SENDER>");

        while (left < message_size){
            std::string msg;
            char msg_c[BUFFER_SIZE];
            size_t len = recv(socket, &msg_c, sizeof(msg_c), 0);
            msg_c[len] = '\0';
            message += std::string(msg_c);
            logger->add_network("RECV", msg_c, "<SENDER>");
            left += BUFFER_SIZE;
        }
        return message;
    }
}


#endif