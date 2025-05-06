#pragma once
#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <map>

#define DEFAULT_PORT "200"
#define MAX_CLIENTS 64
#define BUFFER_SIZE 1024

std::mutex clientMutex;
std::map<SOCKET, char> clientIdentifiers; // 儲存每個 Client 的標識

//namespace tcp
//{
//    class TcpCommute
//    {
//    public:
//        void HandleClient(SOCKET clientSocket);
//    };
//} /* tcp */