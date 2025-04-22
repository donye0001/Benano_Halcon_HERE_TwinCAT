#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <TcAdsDef.h>
#include <TcAdsAPI.h>

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "200"
#define MAX_CLIENTS 64
#define BUFFER_SIZE 1024

namespace tcp
{
    class TcpCommute
    {
    public:
        bool initializeWinsock();
        SOCKET createListenSocket();
        void addClientSocket(SOCKET clientSocket, WSAPOLLFD* fds, int& nfds);
        void removeClient(WSAPOLLFD fds[], int& nfds, int index);
        void cleanup(WSAPOLLFD* fds, int nfds);
    };
} /* tcp */