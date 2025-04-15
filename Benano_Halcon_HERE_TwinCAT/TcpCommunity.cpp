#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>
#include <TcAdsDef.h>
#include <TcAdsAPI.h>
#include "tcp_commute.h"

bool tcp::TcpCommute::initWinsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return false;
    }
    return true;
}

SOCKET tcp::TcpCommute::createListenSocket() {
    struct addrinfo* result = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    int res = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (res != 0) {
        std::cerr << "getaddrinfo failed: " << res << std::endl;
        WSACleanup();
        return INVALID_SOCKET;
    }

    SOCKET listenSocket = socket(result->ai_family,
                                result->ai_socktype,
                                result->ai_protocol);

    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "socket() failed: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return INVALID_SOCKET;
    }

    res = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
    freeaddrinfo(result);
    if (res == SOCKET_ERROR) {
        std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }

    res = listen(listenSocket, SOMAXCONN);
    if (res == SOCKET_ERROR) {
        std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }
    return listenSocket;
}

void tcp::TcpCommute::cleanup(SOCKET listenSocket, SOCKET clientSocket) {
    if (clientSocket != INVALID_SOCKET) closesocket(clientSocket);
    if (listenSocket != INVALID_SOCKET) closesocket(listenSocket);
    WSACleanup();
}