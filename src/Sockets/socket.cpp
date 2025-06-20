#include "socket.h"

Socket::Socket(int version) : version{version}
{
    /*
    version: IPv4->AF_INET
    */
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2,2), &wsadata) != 0)
    {
        std::cerr << "WSA startup failed" << std::endl;
    }
}
Socket::Socket()
{
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2,2), &wsadata) != 0)
    {
        std::cerr << "WSA startup failed" << std::endl;
    }
}

void Socket::Create_socket(int type, int protocol)
{
    /*
    create socket
    input 
        type:   TCP -> SOCK_STREAM
                UDP -> SOCK_DGRAM
        protocol: 0
    */
    int sockfd=socket(version, type, protocol);
    if (sockfd<0)
    {
        std::cerr << "Socket creation failed!" << std::endl;
    }
    sockfd=sockfd;
}

void Socket::connect_socket(char* Address)
{
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);  // Server's port number
    serverAddr.sin_addr.s_addr = inet_addr(Address);  // Server's IP address

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Connect failed!" << std::endl;
    }
}

void Socket::send_msg(char* msg)
{
    send(sockfd, msg, strlen(msg), 0);
}

void Socket::close_socket()
{
    closesocket(sockfd);
    WSACleanup();
}