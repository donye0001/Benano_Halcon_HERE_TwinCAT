#include "ads_append.h"
#include "ads_commute.h"
#include "tcp_package.h"
#include "tcp_commute.h"
#include "data.h"

ads::AdsCommute c_ads;
ads::Linear linear;
gz::realTimeData gzd;

bool running;

void HandleClient(SOCKET clientSocket);

int main() {
    //**ads
    if (!c_ads.isOpened()) //�T�{��J��port
    {
        system("pause");
        return 1;
    }
    c_ads.setTargetAddress(851);
    //ads**


    // ��l�� Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[TCP] WSAStartup failed\n";
        return 1;
    }

    // �إ� socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "[TCP] Socket creation failed\n";
        WSACleanup();
        return 1;
    }

    // �]�w server ��}
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(200); // ��ť port 8080

    // �j�w socket
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "[TCP] Bind failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // �}�l��ť
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "[TCP] Listen failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "[TCP] Server is listening on port 8080...\n";

    std::vector<std::thread> clientThreads;

    running = true;
    while (running)
    {
    //����-ads------------------------------------------------------------------------------------
        long adserror = 0;
        adserror = c_ads.read(ads::index::group::GET, ads::index::get_offset::ANGLE, linear);
        if (linear.resp.arrive)
        {
            //std::cout << "[ads] send sucessfully\n";
            std::cout << "[ADS] take a photo" << std::endl;
        }
    //�ǰe-ads------------------------------------------------------------------------------------
        linear.req.new_data = 1;
        double temp_position[3];
        //temp_position[0] = 100;
        //temp_position[1] = 300;
        //temp_position[2] = 100;
        //for (int i = 0; i < 3; i++)
        //{
        //    linear.req.target_postion[i] = temp_position[i];
        //}
        adserror = 0;
        adserror = c_ads.write(ads::index::group::MOTION, ads::index::motion_offset::LINEAR, linear);
        if (adserror != 0)
        {
            break; //--------error���B�z�覡�٭n�A�h���
        }

        // ���� Client �s�u
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "[TCP] Accept failed\n";
            continue;
        }

        std::cout << "[TCP] New client connected\n";
        // ���C�� Client �Ұʤ@�Ӱ����
        clientThreads.emplace_back(HandleClient, clientSocket);
    }

    // �M�z
    for (auto& thread : clientThreads)
    {
        thread.join();
    }
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}

// �B�z��� Client �����
void HandleClient(SOCKET clientSocket) 
{
    char buffer[1024];
    while (true)
    {
        // �������
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            std::cout << "[TCP] Client disconnected\n";
            closesocket(clientSocket);
            return;
        }

        // �ѪR�Ĥ@�� char �@������
        char identifier = buffer[0];
        {
            std::lock_guard<std::mutex> lock(clientMutex);
            clientIdentifiers[clientSocket] = identifier;
        }
        std::cout << identifier << std::endl;
        // �ھڼ��ѳB�z���
        switch (identifier)
        {
        case 'T': 
        { // DataType1
            DataType10 data;
            memcpy(&data, (char*)buffer, sizeof(DataType10));
            // �N��ƽᤩ��gzd
            gzd.isTouch = data.isTouch;
            gzd.isTake = data.isTake;
            gzd.fullTime = data.fullTime;
            memcpy(gzd.coeffCopy, data.coeffCopy, sizeof(gzd.coeffCopy));

            std::cout << "[TCP] Received from TwinCAT: " << data.rx << ", " << data.fullTime << "\n";
            for (int i = 0; i < 6; i++)
            {
                for (int j = 0; j < 6; j++)
                    std::cout << gzd.coeffCopy[i][j] << " ";
                std::cout << "\n";
            }

            // �B�z�÷ǳƦ^�Ǹ��
            DataType11 transmission;
            transmission.tx = 'T';
            transmission.moveMode = 2;
            transmission.position[0] = 500;
            transmission.position[1] = 0;
            transmission.position[2] = 485.25;
            transmission.position[3] = 0;
            transmission.position[4] = 0;
            transmission.position[5] = 0;
            send(clientSocket, reinterpret_cast<char*>(&transmission), sizeof(DataType11), 0);
            break;
        }
        case 'G': 
        { // DataType2
            DataType20 data;
            memcpy(&data, buffer, sizeof(DataType20));
            std::cout << "[TCP] Received from Client 2: " << data.rx << ", " << data.isOK << "\n";

            // �^��
            DataType21 transmission;
            transmission.tx = 'G';
            transmission.isTouch = gzd.isTouch;
            transmission.isTake = gzd.isTake;
            transmission.fullTime = gzd.fullTime;
            memcpy(transmission.coeffCopy, gzd.coeffCopy, sizeof(transmission.coeffCopy));
            send(clientSocket, reinterpret_cast<char*>(&transmission), sizeof(DataType21), 0);
            break;
        }
        case 'C': 
        { // DataType3
            DataType3 data;
            memcpy(&data, buffer, sizeof(DataType3));
            std::cout << "[TCP] Received from Client 3: " << data.value1 << ", " << data.value2 << ", " << data.value3 << "\n";

            // �^��
            DataType3 response;
            response.identifier = 'C';
            response.value1 = data.value1 * 2;
            response.value2 = !data.value2;
            response.value3 = data.value3 + 1.0;
            send(clientSocket, reinterpret_cast<char*>(&response), sizeof(DataType3), 0);
            break;
        }
        case 'D': 
        { // DataType4 (string �ݭn�S��B�z)
            // ���] string ���שT�w�Φ����׫e��
            DataType4 data;
            data.identifier = 'D';
            int strLen = *(int*)(buffer + 1); // ���] string ���צb buffer[1] �}�l
            data.value1 = std::string(buffer + 5, strLen);
            data.value2 = *(int*)(buffer + 5 + strLen);
            std::cout << "[TCP] Received from Client 4: " << data.value1 << ", " << data.value2 << "\n";

            // �^��
            DataType4 response;
            response.identifier = 'D';
            response.value1 = data.value1 + "_processed";
            response.value2 = data.value2 + 1;
            // �ǦC�� string
            std::vector<char> responseBuffer;
            responseBuffer.push_back('D');
            int strLenResp = response.value1.size();
            responseBuffer.insert(responseBuffer.end(), (char*)&strLenResp, (char*)&strLenResp + sizeof(int));
            responseBuffer.insert(responseBuffer.end(), response.value1.begin(), response.value1.end());
            responseBuffer.insert(responseBuffer.end(), (char*)&response.value2, (char*)&response.value2 + sizeof(int));
            send(clientSocket, responseBuffer.data(), responseBuffer.size(), 0);
            break;
        }
        default:
            std::cout << "[TCP] Unknown identifier: " << identifier << "\n";
        }
    }
}