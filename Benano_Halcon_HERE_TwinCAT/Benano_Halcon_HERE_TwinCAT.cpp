#include "ads_append.h"
#include "ads_commute.h"
#include <TcAdsDef.h>
#include <TcAdsAPI.h>
#include "tcp_commute.h"

//[HINT] 其實linear跟p_tcp是類似概念
ads::AdsCommute c_ads;
tcp::TcpCommute c_tcp;
ads::Linear linear;
tcp::TcpPackage p_tcp;

int main()
{
	//**ADS
	if (!c_ads.isOpened()) //確認輸入的port
	{
		system("pause");
		return 1;
	}
	c_ads.setTargetAddress(851);
	//ADS**

	//**TCP
	if (!c_tcp.initWinsock()) return 1;
	SOCKET listenSocket = c_tcp.createListenSocket();
	if (listenSocket == INVALID_SOCKET) return 1;
	std::cout << "伺服器正在監聽埠 " << DEFAULT_PORT << "..." << std::endl;
	SOCKET clientSocket = accept(listenSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
		c_tcp.cleanup(listenSocket, INVALID_SOCKET);
		return 1;
	}
	std::cout << "客戶端已連接。" << std::endl;
	//TCP**

	// Loop for ADS and TCP
	double temp_position[3];
	char recvbuf[BUFFER_SIZE];
	int recvbuflen = (int)p_tcp.callSize();
	int tcpResult;

	while (1) 
	{
	//傳送-ADS------------------------------------------------------------------------------------
		linear.req.new_data = 1;
		temp_position[0] = 100;
		temp_position[1] = 300;
		temp_position[2] = 100;
		for (int i = 0; i < 3; i++)
		{
			linear.req.target_postion[i] = temp_position[i];
		}
		long adsError = 0;
		adsError = c_ads.write(ads::index::group::MOTION, ads::index::motion_offset::LINEAR, linear);
		if (adsError != 0) 
		{
			break; //--------error的處理方式還要再多思考
		}

	//傳送-TCP------------------------------------------------------------------------------------
		p_tcp.tx = 'T';
		p_tcp.moveMode = 1;
		p_tcp.position[0] = 330;
		p_tcp.position[1] = -150;
		p_tcp.position[2] = 280;
		p_tcp.position[3] = -42;
		p_tcp.position[4] = 0.0;
		p_tcp.position[5] = 45.0;
		
		p_tcp.serialize();
		const char* data = p_tcp.getData();
		size_t dataSize = p_tcp.getSize();
		int sent = send(clientSocket, data, (int)dataSize, 0);
		if (sent == SOCKET_ERROR) 
		{
			std::cerr << "send failed: " << WSAGetLastError() << std::endl;
			break;
		}
		std::cout << "已發送 " << sent << " 位元組回給客戶端。\n";

	//接收-ADS------------------------------------------------------------------------------------
		adsError = 0;
		adsError = c_ads.read(ads::index::group::GET, ads::index::get_offset::ANGLE, linear);
		if (linear.resp.arrive)
		{
			std::cout << "send sucessfully\n";
			linear.resp.arrive = false;
		}

	//接收-TCP------------------------------------------------------------------------------------
		tcpResult = recv(clientSocket, recvbuf, recvbuflen, 0);
		try 
		{
			p_tcp.deserialize(recvbuf, recvbuflen);
			if (p_tcp.rx == 'C') 
			{
				std::cout << "收到來自軟體的資料：" << std::endl;
				std::cout << "rx = " << p_tcp.rx << std::endl;
				std::cout << "fullTime = " << p_tcp.fullTime << std::endl;
				std::cout << "coeffCopy：" << std::endl;
				for (int i = 0; i < 6; ++i)
				{
					for (int j = 0; j < 6; ++j)
					{
						std::cout << p_tcp.coeffCopy[i][j] << " ";
					}
					std::cout << std::endl;
				}
				std::cout << "收到位元組: " << recvbuflen << std::endl;
			}
		}
		catch (const char* e)
		{
			std::cerr << "Deserialization error: " << e << std::endl;
			break;
		}
	} // Loop for TCP and ADS

	// CLOSE
	shutdown(clientSocket, SD_SEND);
	c_tcp.cleanup(listenSocket, clientSocket);

	
	system("PAUSE");
}


