#include "benano_init.h"
#include "benano_halcon.h"
#include "ads_append.h"
#include "ads_commute.h"
#include <TcAdsDef.h>
#include <TcAdsAPI.h>
#include <HalconCpp.h>

ads::AdsCommute commute;
ads::Linear linear;





namespace benano = benano_scansdk;
namespace halcon = HalconCpp;

int main()

{
	double temp_position[6] = { 0 };
	double delta = 0;

	//確認輸入的port
	if (!commute.isOpened()) {
		system("pause");
		return 1;
	}

	commute.setTargetAddress(851);
	loadBenanoDll();
		

	//TODO: Connect TwinCAT

	// Connect Benano
	benano::CScanManager camManager = initCameraManager(benano::eSDKMovableDeviceType::SDK_NoDevice);
	printCameraInfo(camManager);
	benano::CScanCtrl& scanCtl = camManager.GetScanController();
	benano::CLiveCtrl& liveCtl = camManager.GetLiveController();
	benano::CParameterCtrl& paramCtl = camManager.GetParameterController();
	// Set Parameters
	liveCtl.SetBrightness(57);
	liveCtl.SetCameraGain(0);
	liveCtl.SetExposureMode(benano::eExposureMode::eExposureMode_Short);
	liveCtl.InitialLive(benano::eLiveCtrlType::LiveCheckBrightness);
	benano::CScanParam* scanParam = paramCtl.GetScanParam();
	scanParam->SetScanMode(benano::eScanMode::eScanMode_Confirm);
	scanParam->SetImagingMode(benano::eImagingMode::eImagingMode_Standard);
	scanParam->SetEnableNormalCalculation(true);
	scanParam->SetEnableOutputMesh(true);
	scanParam->SetEnablePointCloudColorOutput(true);
	scanParam->SetEnableRGBLight(true);
	paramCtl.ApplyScanParam(scanParam);
	benano::CPostProcessParam* postParam = paramCtl.GetPostProcessParam();
	postParam->Data.bBottomLevel = true;
	postParam->Data.bTopLevel = true;
	postParam->Data.BottomLevel = 99.5f;
	postParam->Data.TopLevel = 107.0f;
	postParam->Data.bBaseCorrection = true;
	postParam->Data.SmoothIteration = 3;
	paramCtl.ApplyPostProcessParam(postParam);
	benano::CAlgorithm3DParam* algoParam = paramCtl.GetAlgorithm3DParam();
	paramCtl.ApplyAlgorithm3DParam(algoParam);

	// TODO: Halcon socket server
	// Command: Benano->1000~1999, TwinCAT->2000->2999
	halcon::HTuple serverData, serverImage, socketData, sendFrom, sendTo, command;
	halcon::OpenSocketAccept(3000, "protocol", "TCP4", &serverData);
	std::cout << "[Info]Listening TCP4 port 3000\n";
	halcon::SocketAcceptConnect(serverData, "true", &socketData);
	std::cout << "[Info]Get clients\n";
	while (true) {
		try {
			halcon::ReceiveData(socketData, "i", &command, &sendFrom);
			std::cout << "[Info]Get command\n";
			// TODO: Throw error or return error code
			std::cout << command[0].ToTuple().TupleIsInt().I() << ' ' << command.I() << std::endl;
			if (command[0].ToTuple().TupleIsInt().I()) {
				int cmd = command.I();
				if (cmd >= 1000 && cmd < 2000)
					benanoStuff(socketData, command, camManager);
				// REMIND: TwinCAT指令接收
				//else if (cmd >= 2000 && cmd < 3000)
				//	twincatStuff(socketData, command);
			}
		}
		catch (halcon::HException ex) {
			std::cerr << "[Error]Halcon: " << ex.ErrorMessage() << std::endl << std::endl;
			if (ex.ErrorCode() == 5606) {  // Loss Socket Connection
				break;
			}
		}
		catch (std::exception ex) {
			std::cerr << "[Error]Command " << command.ToString() << ": " << ex.what() << std::endl << std::endl;
			break;
		}
		catch (...) {
			// TODO: Return error code
			std::cout << "[Error]Unknown\n\n";
			break;
		}
	}

	std::cout << "[Info]Close server\n";
	halcon::CloseSocket(socketData);
	system("PAUSE");
}


