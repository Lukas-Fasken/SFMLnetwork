#ifndef SOCKET_H
#define SOCKET_H
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>


inline int InitializeWinsock()
{
    // Call this function at the start of your program to initialize Winsock
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2,2), &wsadata) != 0)
    {
        std::cerr << "WSA startup failed" << std::endl;
        return -1;
    }
    return 0;
}

class Socket
{
    public:
    Socket(int version);
    Socket();
    void Create_socket(int type, int protocol);
    void connect_socket(char* Address);
    void send_msg(char* msg);
    void close_socket();


    int sockfd;
    private:
    const int version {AF_INET};
};

#endif