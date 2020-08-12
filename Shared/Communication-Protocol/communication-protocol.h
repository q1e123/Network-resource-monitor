#ifndef COMMUNICATION_PROTOCOL_H
#define COMMUNICATION_PROTOCOL_H
#include <string>

#include <algorithm>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <vector>
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

class Socket_Error_Exception: public std::exception{
  virtual const char* what() const throw(){
    return "Unknown socket error";
  }
};

class Client_Down_Exception: public std::exception{
  virtual const char* what() const throw(){
    return "Client down";
  }
};

namespace Communication_Protocol{
    std::string add_size(std::string message){
        std::stringstream stream;
        stream << std::setw(10) << std::setfill('0') << message.size();
        std::string encapsulated_size = stream.str();
        message = encapsulated_size + message;
        return message;
    }

    size_t decapsulate_size(std::string encapsulated_size){
        encapsulated_size.erase(0, std::min(encapsulated_size.find_first_not_of('0'), encapsulated_size.size()-1));
        size_t size;
        if(encapsulated_size.size() < 1){
            return 0;
        }
        try{
            size= std::stol(encapsulated_size);
        }catch(const std::exception& e){
            std::cerr << encapsulated_size << " " << e.what() << '\n';
        }        
              
        return size;
    }

    void send_message(SOCKET socket, std::string message, Logger *logger){
        std::string encapsulated_string = message;
        size_t total_bytes_sent = 0;
        size_t bytes_sent = 0;
        size_t size = encapsulated_string.size();
        send(socket, &size, sizeof(size), 0);
        while (total_bytes_sent < encapsulated_string.size()){
            std::string message_left = encapsulated_string.substr(bytes_sent);
            bytes_sent = send(socket, message_left.c_str(), sizeof(message_left.c_str()), 0);
            total_bytes_sent += bytes_sent;
            logger->add_network("SEND", encapsulated_string.substr(total_bytes_sent - bytes_sent, total_bytes_sent), "<RECIVER>");
            if(bytes_sent < 0){
                logger->add_error("MESSAGE NOT SENT");
                return;
            }
        }
                            
    }
    std::string get_message(SOCKET socket, size_t size){
        std::string message = "";
        char *recv_buffer = new char[BUFFER_SIZE];
        size_t bytes_recived = 0;
        while (bytes_recived < size){
            memset(recv_buffer, '\0', BUFFER_SIZE);
            size_t len = recv(socket, recv_buffer, size - bytes_recived, 0);
            if(len < 0){
                throw Socket_Error_Exception();
            }
            if(len == 0){
                throw Client_Down_Exception();
            }
            recv_buffer[len] = '\0';
            std::string recv_msg = std::string(recv_buffer); 
            message += recv_msg;
            if(recv_msg.size() > 1){
                //logger->add_network("RECV", recv_msg, "<SENDER>");
            }
            bytes_recived += len;
        }
        delete recv_buffer;
        return message;        
    }
    std::string recv_message(SOCKET socket, Logger *logger){
        size_t message_size;
        recv(socket, &message_size, sizeof(message_size), 0);
        logger->add_network("RECV", "SIZE = " + std::to_string(message_size), "<SENDER>");
        std::string message = get_message(socket, message_size);
        logger->add_network("RECV", "BODY = " + message, "<SENDER>");
        return message;
    }
    
}


#endif