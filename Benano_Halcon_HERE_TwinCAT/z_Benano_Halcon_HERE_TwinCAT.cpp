//#include "ads_append.h"
//#include "ads_commute.h"
//#include <tcadsdef.h>
//#include <tcadsapi.h>
//#include "tcp_commute.h"
//#include "tcp_package.h"
//#include <map>
//
//#define idlen 1
//
////[hint] 其實linear跟tpp是類似概念
//ads::adscommute c_ads;
//tcp::tcpcommute c_tcp;
//ads::linear linear;
//tpp::tcletter tcl;
//tpp::gzletter gzl;
//
//int main()
//{
//	//**ads
//	if (!c_ads.isopened()) //確認輸入的port
//	{
//		system("pause");
//		return 1;
//	}
//	c_ads.settargetaddress(851);
//	//ads**
//
//	//**tcp
//	if (!c_tcp.initializewinsock()) {
//		std::cerr << "[tcp] 初始化 winsock 失敗。\n";
//		return 1;
//	}
//	socket listensocket = c_tcp.createlistensocket();
//	if (listensocket == invalid_socket) {
//		wsacleanup();
//		return 1;
//	}
//	// 建立伺服端
//	wsapollfd fds[max_clients] = {};
//	fds[0].fd = listensocket;
//	fds[0].events = pollrdnorm;
//	int nfds = 1;
//	std::cout << "[tcp] 伺服器正在監聽埠 " << default_port << "...\n";
//
//	// 儲存客戶端資訊
//	std::map<socket, char> client_ids; // socket -> client_id
//	std::map<char, int> client_data = {
//		{'t', 11},
//		{'g', 21},
//		{'h', 31}};
//
//	//tcp**
//
//	// loop for ads and tcp
//	double temp_position[3];
//	bool running = 1;
//
//	while (running)
//	{
//	//接收-ads------------------------------------------------------------------------------------
//		long adserror = 0;
//		adserror = c_ads.read(ads::index::group::get, ads::index::get_offset::angle, linear);
//		if (linear.resp.arrive)
//		{
//			//std::cout << "[ads] send sucessfully\n";
//			linear.resp.arrive = false;
//		}
//
//
//	//傳送-ads------------------------------------------------------------------------------------
//		linear.req.new_data = 1;
//		temp_position[0] = 100;
//		temp_position[1] = 300;
//		temp_position[2] = 100;
//		for (int i = 0; i < 3; i++)
//		{
//			linear.req.target_postion[i] = temp_position[i];
//		}
//		adserror = 0;
//		adserror = c_ads.write(ads::index::group::motion, ads::index::motion_offset::linear, linear);
//		if (adserror != 0)
//		{
//			break; //--------error的處理方式還要再多思考
//		}
//
//
//		int j = 2;
//	// tcp 處理poll
//		int pollresult = wsapoll(fds, nfds, -1);
//		if (pollresult == socket_error)
//		{
//			std::cerr << "[tcp] wsapoll failed: " << wsagetlasterror() << "\n";
//			break;
//		}
//
//	// tcp 處理new client
//		if (fds[0].revents & pollrdnorm) {
//			socket clientsocket = accept(listensocket, nullptr, nullptr);
//			if (clientsocket != invalid_socket && nfds < max_clients) {
//				fds[nfds].fd = clientsocket;
//				fds[nfds].events = pollrdnorm;
//				nfds++;
//				std::cout << "[tcp] new client connected: " << clientsocket << "\n";
//			}
//			else {
//				std::cerr << "[tcp] accept failed or max clients reached\n";
//				closesocket(clientsocket);
//			}
//		}
//
//		char recvbuf[buffer_size];
//		int recvbuflen = 1024;
//		for (int i = 1; i < nfds; i++) 
//		{
//			if (fds[i].revents & pollrdnorm) 
//			{
//				socket clientsocket = fds[i].fd;
//				int bytesreceived = recv(clientsocket, recvbuf, recvbuflen, 0);
//				if (bytesreceived > 0) 
//				{
//					tcl.deserialize(recvbuf, recvbuflen);
//					if (client_ids.find(clientsocket) == client_ids.end()) 
//					{
//						// 接收 client_id
//						std::string client_id = std::string(buffer, bytesreceived);
//						client_ids[clientsocket] = client_id;
//						std::cout << "[server] client " << clientsocket << " registered as: " << client_id << "\n";
//
//						// 發送對應的 vector
//						auto it = client_data.find(client_id);
//						if (it != client_data.end()) 
//						{
//							std::vector<char> data = serialize_vector(it->second);
//							int sent = send(clientsocket, data.data(), static_cast<int>(data.size()), 0);
//							if (sent == socket_error) 
//							{
//								std::cerr << "[server] send failed: " << wsagetlasterror() << "\n";
//								closesocket(clientsocket);
//								client_ids.erase(clientsocket);
//								fds[i].fd = -1; // 標記為無效
//							}
//							else 
//							{
//								std::cout << "[server] sent vector to " << client_id << ": ";
//								for (int val : it->second) std::cout << val << " ";
//								std::cout << "\n";
//							}
//						}
//						else 
//						{
//							std::cerr << "[server] unknown client_id: " << client_id << "\n";
//						}
//					}
//					else 
//					{
//						// 接收客戶端回應
//						std::string response(buffer, bytesreceived);
//						std::cout << "[server] received from " << client_ids[clientsocket] << ": " << response << "\n";
//					}
//				}
//				else if (bytesreceived == 0)
//				{
//					std::cout << "[server] client " << client_ids[clientsocket] << " disconnected\n";
//					closesocket(clientsocket);
//					client_ids.erase(clientsocket);
//					fds[i].fd = -1; // 標記為無效
//				}
//				else 
//				{
//					std::cerr << "[server] recv failed: " << wsagetlasterror() << "\n";
//					closesocket(clientsocket);
//					client_ids.erase(clientsocket);
//					fds[i].fd = -1; // 標記為無效
//				}
//			}
//		}
//
//
//		for (int i = 1; i < nfds; ++i)
//		{
//			if (fds[i].revents & pollrdnorm)
//			{
//	//傳送-tcp------------------------------------------------------------------------------------
//				tcl.tx = 'c';
//				tcl.movemode = 1;
//				tcl.position[0] = 330;
//				tcl.position[1] = -150;
//				tcl.position[2] = 280;
//				tcl.position[3] = -42;
//				tcl.position[4] = 0.0;
//				tcl.position[5] = 45.0;
//
//				tcl.serialize();
//				const char* data = tcl.getdata();
//				size_t datasize = tcl.getsize();
//
//				int sent = send(fds[i].fd, data, (int)datasize, 0);
//				if (sent == socket_error)
//				{
//					std::cerr << "[tcp] send failed: " << wsagetlasterror() << std::endl;
//					break;
//				}
//				//std::cout << "[tcp] 已發送 " << sent << " 位元組回給客戶端。\n";
//
//	//接收-tcp------------------------------------------------------------------------------------
//				recvbuflen = max((int)tcl.callsize(), (int)gzl.callsize());
//				int tcpresult = recv(fds[i].fd, recvbuf, recvbuflen, 0);
//				std::cout << "[tcp] data = " << tcpresult << " bits, recvbuff = " << recvbuf << ". \n";
//				if (tcpresult > 0)
//				{
//					try
//					{
//						if (recvbuf[0] != 'c')
//						{
//							recvbuflen = (int)tcl.callsize();
//							tcl.deserialize(recvbuf, recvbuflen);
//							std::cout << "[tcp] 收到資料 rx = " << tcl.rx << ", fulltime = " << tcl.fulltime << "\n";
//						}
//					}
//					catch (const char* e)
//					{
//						std::cerr << "[tcp] deserialization error: " << e << std::endl;
//					}
//				}
//				else if (tcpresult == 0) {
//					std::cout << "[tcp] 客戶端關閉連線。\n";
//					c_tcp.removeclient(fds, nfds, i--);
//				}
//				else {
//					std::cerr << "[tcp] recv 錯誤: " << wsagetlasterror() << "\n";
//					c_tcp.removeclient(fds, nfds, i--);
//				}
//			}
//		} 
//	} // loop for tcp and ads
//	c_tcp.cleanup(fds, nfds);
//	system("pause");
//}
//
//
