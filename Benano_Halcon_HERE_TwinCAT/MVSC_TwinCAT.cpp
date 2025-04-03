#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>
#include <TcAdsDef.h>
#include <TcAdsAPI.h>
#include "testChar.cpp"

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "200"
#define BUFFER_SIZE 1024

class tc_port
{
    public:
        std::vector<char> letter;    // 儲存序列化後的資料
        char tx;                     // 傳送
        int moveMode;                // 整數
        double position[6];          // 浮點數陣列
        char rx;                     // 接收
        double fullTime;                
        double coeffCopy[6][6];        

        void serialize() 
        {
            letter.clear();
            appendToLetter(&tx, sizeof(tx));
            appendToLetter(&moveMode, sizeof(moveMode));
            appendToLetter(&position, sizeof(position));
        }

        void deserialize(void* src, size_t size)
        {
            if (size != this->callSize()) 
            {
                std::cout << size << std::endl;
                std::cout << this->getSize() << std::endl;
                throw("Wrong src's size for deserialization.");
            }

            // 依對應成員的型態選擇不同的*(type*)方式容易打錯，改用memcpy與位元組讀取計數
            size_t count = 0;
            memcpy(&this->rx, (unsigned char*)src + count, sizeof(this->rx));
            count += sizeof(this->rx);
            memcpy(&this->fullTime, (unsigned char*)src + count, sizeof(this->fullTime));
            count += sizeof(this->fullTime);
            memcpy(&this->coeffCopy, (unsigned char*)src + count, sizeof(this->coeffCopy));
        }

        // 提供只讀訪問方式
        const char* getData() const
        {
            return letter.data();
        }

        size_t getSize()
        {
            return letter.size();
        }

        size_t callSize()
        {
            return sizeof(rx) + sizeof(fullTime) + sizeof(coeffCopy);
        }

    private:
        // 輔助函數：將任意型別的資料附加到字節向量
        template <typename T>
        void appendToLetter(const T* data, size_t size) 
        {
            const char* bytePtr = reinterpret_cast<const char*>(data);
            letter.insert(letter.end(), bytePtr, bytePtr + size);
        }
};

//void tcko()
//{
//    tc_port tc_port;
//    tc_port.tx = 'T';
//    tc_port.fullTime = 10;
//    tc_port.position[0] = 1.2;
//    tc_port.position[1] = 3.33;
//    tc_port.position[2] = 10.7;
//    tc_port.position[3] = 5.85;
//    tc_port.position[4] = 3.654;
//    tc_port.position[5] = 3.3333333333333;
//
//    int size1 = sizeof(tc_port.rx) + sizeof(tc_port.fullTime) + sizeof(tc_port.coeffCopy);
//    std::cout << size1 << std::endl;
//    tc_port.serialize();
//}


int main() {
    WSADATA wsaData;
    int iResult;

    //tcko();
    //return 0;
    
    // 初始化 Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    struct addrinfo* result = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_protocol = IPPROTO_TCP; // TCP
    hints.ai_flags = AI_PASSIVE;     // 用於綁定的被動套接字

    // 解析本地埠以進行監聽
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) 
    {
        std::cerr << "getaddrinfo failed: " << iResult << std::endl;
        WSACleanup();
        return 1;
    }

    // 創建監聽套接字
    SOCKET ListenSocket = INVALID_SOCKET;
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) 
    {
        std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // 綁定套接字
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    // 開始監聽
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) 
    {
        std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "伺服器正在監聽埠 " << DEFAULT_PORT << "..." << std::endl;

    // 接受客戶端連接
    SOCKET ClientSocket = INVALID_SOCKET;
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) 
    {
        std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "客戶端已連接。" << std::endl;

    // 接收和回送數據
    tc_port tc_port;
    char recvbuf[BUFFER_SIZE];
    int recvbuflen = tc_port.callSize();
    int counter = 0;

    tc_port.tx = 'T';
    tc_port.moveMode = 1;
    tc_port.position[0] = 330;
    tc_port.position[1] = -150;
    tc_port.position[2] = 280;
    tc_port.position[3] = -42;
    tc_port.position[4] = 0.0;
    tc_port.position[5] = 45.0;
    size_t count = 0;

    do 
    {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        tc_port.deserialize(recvbuf, recvbuflen);
        if (iResult > 0)
        {
            if (tc_port.rx == 'C')
            {
                std::cout << "Deserialize\n";
                std::cout << "來自軟體" << tc_port.rx << "\n";
                std::cout << "需要時間" << tc_port.fullTime << "\n";
                std::cout << "公式係數" << "\n";
                for (int i = 0; i < 6; i++)
                {
                    for (int j = 0; j < 6; j++)
                    {
                        std::cout << tc_port.coeffCopy[i][j] << " ";
                    }
                    std::cout << "\n";
                }
                std::cout << "收到位元組: " << recvbuflen << std::endl;
            }
           
            
            tc_port.serialize();
            const char* data = tc_port.getData();
            size_t dataSize = tc_port.getSize();
            int iSendResult = send(ClientSocket, data, dataSize, 0);
            if (iSendResult == SOCKET_ERROR)
            {
                std::cerr << "send failed: " << WSAGetLastError() << std::endl;
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }

            std::cout << "已發送 " << iSendResult << " 位元組回給客戶端。" << std::endl;
        }
        else if (iResult == 0)
            std::cout << "連接關閉。" << std::endl;
        else 
        {
            std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }
    } while (iResult > 0);

    // 關閉客戶端套接字
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        std::cerr << "shutdown failed: " << WSAGetLastError() << std::endl;
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    closesocket(ClientSocket);
    closesocket(ListenSocket);
    WSACleanup();

    return 0;
}