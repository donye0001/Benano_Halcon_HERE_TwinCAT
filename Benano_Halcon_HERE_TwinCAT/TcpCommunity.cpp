#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>
#include <TcAdsDef.h>
#include <TcAdsAPI.h>
#include "tcp_commute.h"


bool tcp::TcpCommute::initializeWinsock() {
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

SOCKET tcp::TcpCommute::createListenSocket() {
    struct addrinfo* result = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, DEFAULT_PORT, &hints, &result) != 0) {
        std::cerr << "[TCP] getaddrinfo failed.\n";
        return INVALID_SOCKET;
    }

    SOCKET listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "[TCP] socket creation failed.\n";
        freeaddrinfo(result);
        return INVALID_SOCKET;
    }

    if (bind(listenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
        std::cerr << "[TCP] bind failed.\n";
        freeaddrinfo(result);
        closesocket(listenSocket);
        return INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "[TCP] listen failed.\n";
        closesocket(listenSocket);
        return INVALID_SOCKET;
    }
    return listenSocket;
}

void tcp::TcpCommute::addClientSocket(SOCKET clientSocket, WSAPOLLFD* fds, int& nfds) {
    if (nfds < MAX_CLIENTS) {
        fds[nfds].fd = clientSocket;
        fds[nfds].events = POLLRDNORM;
        nfds++;
    }
    else {
        std::cerr << "[TCP] Too many clients.\n";
        closesocket(clientSocket);
    }
}

void tcp::TcpCommute::removeClient(WSAPOLLFD fds[], int& nfds, int index) {
    closesocket(fds[index].fd);
    for (int j = index; j < nfds - 1; j++)
        fds[j] = fds[j + 1];
    nfds--;
}

void tcp::TcpCommute::cleanup(WSAPOLLFD* fds, int nfds) {
    for (int i = 0; i < nfds; i++)
        closesocket(fds[i].fd);
    WSACleanup();
}