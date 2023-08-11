#include "SendByte.h"
#include <Ws2tcpip.h>

int Send(char data)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "socket creation failed" << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2001); // Convert port to network byte order

    if (inet_pton(AF_INET, "192.168.1.1", &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "connect failed" << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    char byteToSend = data; // Change this to the byte you want to send
    if (send(sockfd, &byteToSend, sizeof(byteToSend), 0) == SOCKET_ERROR) {
        std::cerr << "send failed" << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    closesocket(sockfd);
    WSACleanup();
    return 0;
}

