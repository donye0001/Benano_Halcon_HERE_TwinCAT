//#include "tcp_new_package.h"
//#include "tcp_new_commute.h"
//
//// 處理單個 Client 的函數
//void tcp::TcpCommute::HandleClient(SOCKET clientSocket) {
//    char buffer[1024];
//    while (true) {
//        // 接收資料
//        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
//        if (bytesReceived <= 0) {
//            std::cout << "[TCP] Client disconnected\n";
//            closesocket(clientSocket);
//            return;
//        }
//
//        // 解析第一個 char 作為標識
//        char identifier = buffer[0];
//        {
//            std::lock_guard<std::mutex> lock(clientMutex);
//            clientIdentifiers[clientSocket] = identifier;
//        }
//
//        // 根據標識處理資料
//        switch (identifier) {
//        case 'T': { // DataType1
//            DataType10 data;
//            memcpy(&data, buffer, sizeof(DataType10));
//            std::cout << "Received from Client 1: " << data.fullTime << ", " << data.coeffCopy << "\n";
//
//            // 處理並準備回傳資料
//            DataType11 transmission;
//            transmission.tx = 'A';
//            transmission.moveMod = data.fullTime + 1;
//            transmission.position[0] = 500;
//            transmission.position[1] = 0;
//            transmission.position[2] = 485.55f;
//            transmission.position[3] = 0;
//            transmission.position[4] = 0;
//            transmission.position[5] = 0;
//            send(clientSocket, reinterpret_cast<char*>(&transmission), sizeof(DataType11), 0);
//            break;
//        }
//        case 'B': { // DataType2
//            DataType2 data;
//            memcpy(&data, buffer, sizeof(DataType2));
//            std::cout << "Received from Client 2: " << data.value1 << ", " << data.value2 << "\n";
//
//            // 回傳
//            DataType2 response;
//            response.identifier = 'B';
//            response.value1 = data.value1 + 1.0f;
//            response.value2 = !data.value2;
//            send(clientSocket, reinterpret_cast<char*>(&response), sizeof(DataType2), 0);
//            break;
//        }
//        case 'C': { // DataType3
//            DataType3 data;
//            memcpy(&data, buffer, sizeof(DataType3));
//            std::cout << "Received from Client 3: " << data.value1 << ", " << data.value2 << ", " << data.value3 << "\n";
//
//            // 回傳
//            DataType3 response;
//            response.identifier = 'C';
//            response.value1 = data.value1 * 2;
//            response.value2 = !data.value2;
//            response.value3 = data.value3 + 1.0;
//            send(clientSocket, reinterpret_cast<char*>(&response), sizeof(DataType3), 0);
//            break;
//        }
//        case 'D': { // DataType4 (string 需要特殊處理)
//            // 假設 string 長度固定或有長度前綴
//            DataType4 data;
//            data.identifier = 'D';
//            int strLen = *(int*)(buffer + 1); // 假設 string 長度在 buffer[1] 開始
//            data.value1 = std::string(buffer + 5, strLen);
//            data.value2 = *(int*)(buffer + 5 + strLen);
//            std::cout << "Received from Client 4: " << data.value1 << ", " << data.value2 << "\n";
//
//            // 回傳
//            DataType4 response;
//            response.identifier = 'D';
//            response.value1 = data.value1 + "_processed";
//            response.value2 = data.value2 + 1;
//            // 序列化 string
//            std::vector<char> responseBuffer;
//            responseBuffer.push_back('D');
//            int strLenResp = response.value1.size();
//            responseBuffer.insert(responseBuffer.end(), (char*)&strLenResp, (char*)&strLenResp + sizeof(int));
//            responseBuffer.insert(responseBuffer.end(), response.value1.begin(), response.value1.end());
//            responseBuffer.insert(responseBuffer.end(), (char*)&response.value2, (char*)&response.value2 + sizeof(int));
//            send(clientSocket, responseBuffer.data(), responseBuffer.size(), 0);
//            break;
//        }
//        default:
//            std::cout << "Unknown identifier: " << identifier << "\n";
//        }
//    }
//}