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
        std::vector<char> letter;    // �x�s�ǦC�ƫ᪺���
        char tx;                     // �ǰe
        int moveMode;                // ���
        double position[6];          // �B�I�ư}�C
        char rx;                     // ����
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

            // �̹������������A��ܤ��P��*(type*)�覡�e�������A���memcpy�P�줸��Ū���p��
            size_t count = 0;
            memcpy(&this->rx, (unsigned char*)src + count, sizeof(this->rx));
            count += sizeof(this->rx);
            memcpy(&this->fullTime, (unsigned char*)src + count, sizeof(this->fullTime));
            count += sizeof(this->fullTime);
            memcpy(&this->coeffCopy, (unsigned char*)src + count, sizeof(this->coeffCopy));
        }

        // ���ѥuŪ�X�ݤ覡
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
        // ���U��ơG�N���N���O����ƪ��[��r�`�V�q
        template <typename T>
        void appendToLetter(const T* data, size_t size)
        {
            const char* bytePtr = reinterpret_cast<const char*>(data);
            letter.insert(letter.end(), bytePtr, bytePtr + size);
        }
    };

} /* tcp */