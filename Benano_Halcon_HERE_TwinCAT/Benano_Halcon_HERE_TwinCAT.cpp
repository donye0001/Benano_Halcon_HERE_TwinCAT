#include "ads_append.h"
#include "ads_commute.h"
#include <TcAdsDef.h>
#include <TcAdsAPI.h>
#include "tcp_commute.h"
#include "tcp_package.h"

//[HINT] 其實linear跟tpp是類似概念
ads::AdsCommute c_ads;
tcp::TcpCommute c_tcp;
ads::Linear linear;
tpp::TcLetter tcl;
tpp::GzLetter gzl;

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
	if (!c_tcp.initializeWinsock()) {
		std::cerr << "[TCP] 初始化 Winsock 失敗。\n";
		return 1;
	}
	SOCKET listenSocket = c_tcp.createListenSocket();
	if (listenSocket == INVALID_SOCKET) {
		WSACleanup();
		return 1;
	}
	WSAPOLLFD fds[MAX_CLIENTS] = {};
	fds[0].fd = listenSocket;
	fds[0].events = POLLRDNORM;
	int nfds = 1;
	std::cout << "[TCP] 伺服器正在監聽埠 " << DEFAULT_PORT << "...\n";
	//TCP**

	// Loop for ADS and TCP
	double temp_position[3];
	char recvbuf[BUFFER_SIZE];
	int recvbuflen;
	bool running = 1;

	while (running)
	{
	// TCP 處理poll
		int pollResult = WSAPoll(fds, nfds, -1);
		if (pollResult == SOCKET_ERROR)
		{
			std::cerr << "[TCP] WSAPoll failed: " << WSAGetLastError() << "\n";
			break;
		}
	// TCP 處理new client
		if (fds[0].revents & POLLRDNORM) 
		{
			SOCKET clientSocket = accept(listenSocket, NULL, NULL);
			if (clientSocket != INVALID_SOCKET && nfds < MAX_CLIENTS) {

				fds[nfds].fd = clientSocket;
				fds[nfds].events = POLLRDNORM;
				nfds++;
				std::cout << "[TCP] 新客戶端已連接: " << clientSocket << "\n";
			}
			else {
				std::cerr << "[TCP] 接收連線失敗或已滿。\n";
			}
		}

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

	//接收-ADS------------------------------------------------------------------------------------
		adsError = 0;
		adsError = c_ads.read(ads::index::group::GET, ads::index::get_offset::ANGLE, linear);
		if (linear.resp.arrive)
		{
			//std::cout << "[ADS] send sucessfully\n";
			linear.resp.arrive = false;
		}

		for (int i = 1; i < nfds; ++i)
		{
			if (fds[i].revents & POLLRDNORM)
			{
	//傳送-TCP------------------------------------------------------------------------------------
				tcl.tx = 'C';
				tcl.moveMode = 1;
				tcl.position[0] = 330;
				tcl.position[1] = -150;
				tcl.position[2] = 280;
				tcl.position[3] = -42;
				tcl.position[4] = 0.0;
				tcl.position[5] = 45.0;

				tcl.serialize();
				const char* data = tcl.getData();
				size_t dataSize = tcl.getSize();

				int sent = send(fds[i].fd, data, (int)dataSize, 0);
				if (sent == SOCKET_ERROR)
				{
					std::cerr << "[TCP] send failed: " << WSAGetLastError() << std::endl;
					break;
				}
				//std::cout << "[TCP] 已發送 " << sent << " 位元組回給客戶端。\n";

	//接收-TCP------------------------------------------------------------------------------------
				recvbuflen = std::max((int)tcl.callSize(), (int)gzl.callSize());
				int tcpResult = recv(fds[i].fd, recvbuf, recvbuflen, 0);
				std::cout << "[TCP] DATA = " << tcpResult << " bits, RecvBuff = " << recvbuf << ". \n";
				if (tcpResult > 0)
				{
					try
					{
						if (recvbuf[0] == 'C')
						{
							recvbuflen = (int)tcl.callSize();
							tcl.deserialize(recvbuf, recvbuflen);
							std::cout << "[TCP] 收到資料 rx = " << tcl.rx << ", fullTime = " << tcl.fullTime << "\n";
						}
					}
					catch (const char* e)
					{
						std::cerr << "[TCP] Deserialization error: " << e << std::endl;
					}
				}
				else if (tcpResult == 0) {
					std::cout << "[TCP] 客戶端關閉連線。\n";
					c_tcp.removeClient(fds, nfds, i--);
				}
				else {
					std::cerr << "[TCP] recv 錯誤: " << WSAGetLastError() << "\n";
					c_tcp.removeClient(fds, nfds, i--);
				}
			}
		} 
	} // Loop for TCP and ADS
	c_tcp.cleanup(fds, nfds);
	system("PAUSE");
}


