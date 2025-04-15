#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>
#include <TcAdsDef.h>
#include <TcAdsAPI.h>

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "200"
#define BUFFER_SIZE 1024

namespace tcp
{
    class TcpCommute
    {
    public:
        bool initWinsock();
        SOCKET createListenSocket();
        void cleanup(SOCKET listenSocket, SOCKET clientSocket);
    };

    class TcpPackage
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

} /* tcp */