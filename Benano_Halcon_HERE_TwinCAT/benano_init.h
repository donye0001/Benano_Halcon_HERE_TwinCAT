#pragma once
#include <Windows.h>
#include <ScanSDK/BnScanSDK.h>
#include <iostream>


void loadBenanoDll()
{
#if _DEBUG
	SetDllDirectory(L"C:\\Program Files\\Benano\\BenanoScanSDK\\Bin\\Debug");
#else
	SetDllDirectory(L"C:\\Program Files\\Benano\\BenanoScanSDK\\Bin\\Release");
#endif
	LoadLibrary(L"BenanoScanSDK.dll");
}

void connectCamera(benano_scansdk::CScanManager& manager, benano_scansdk::CInitialParam& param)
{
	bool initFailed = false;
	while (true) {
		std::cout << "[Info]Connecting Benano's camera...\n";
		try {
			if (initFailed)
				manager.Reconnect();
			else
				manager.InitialScanner(param);

			if (!manager.GetDeviceInfo().GetCameraCount())
				throw std::exception("Camera count is 0.");
			break;

		}
		catch (std::exception ex) {
			initFailed = true;
			std::cerr << "[Error]Benano: " << ex.what();
			system("PAUSE");
			std::cout << std::endl;
		}
	}
	std::cout << "[Info]Camera connection successful.\n";
}

benano_scansdk::CScanManager& initCameraManager(benano_scansdk::eSDKMovableDeviceType movableDeviceType, benano_scansdk::U8 solverCount = 1,
	std::string calibratePath = "", std::string logPath = "", int KeepLogFileInDays = 7)
{
	benano_scansdk::CScanManager* manager = new benano_scansdk::CScanManager;
	benano_scansdk::CInitialParam camInitParam;
	/*camInitParam.CalibrationFilePath = calibratePath;
	camInitParam.LogFilePath = logPath;
	camInitParam.MaxSolverCnt = solverCount;
	camInitParam.MovableDeviceType = movableDeviceType;
	camInitParam.KeepLogFileInDays = KeepLogFileInDays;*/
	connectCamera(*manager, camInitParam);
	return *manager;
}

void printDescription(benano_scansdk::CDeviceInfo& info, size_t cameraCount)
{
	std::cout << "Device description:\n";
	std::cout << "\tType: " << info.GetModuleType() << '\n';
	std::cout << "\tName: " << info.GetModelName() << '\n';
	std::cout << "\tSerial number: " << info.GetSerialNumber() << '\n';
	std::cout << "Camera:\n";
	std::cout << "\tCount: " << cameraCount << '\n';
	for (size_t i = 0; i < cameraCount; i++) {
		std::cout << "\tCam " << i << ": \n";
		std::cout << "\t\tWidth: " << info.GetImageWidth(i) << '\n';
		std::cout << "\t\tHeight: " << info.GetImageHeight(i) << '\n';
		std::cout << "\t\tX resolution: " << info.GetCameraResolutionX(i) << '\n';
		std::cout << "\t\tY resolution: " << info.GetCameraResolutionY(i) << '\n';
	}
}

void printAuthority(benano_scansdk::DongleKeyAuthority& dongle)
{
	std::cout << "Authority:\n";
	std::cout << "\tEnable Anti-reflection: " << dongle.IsEnableAntiReflection << '\n';
	std::cout << "\tEnable Set exposure mode: " << dongle.IsEnableSetExposureMode << '\n';
	std::cout << "\tEnable Set imaging mode: " << dongle.IsEnableSetImagingMode << '\n';
}

void printCameraInfo(benano_scansdk::CScanManager& manager)
{
	benano_scansdk::CCalibrator& calibrator = manager.GetCalibrator();
	benano_scansdk::DongleKeyAuthority& dongle = manager.GetDongleKeyAuthority();
	size_t cameraCount = manager.GetDeviceInfo().GetCameraCount();
	std::cout << "[Info]Benano static information\n";
	std::cout << "SDK version: " << manager.GetVersion() << '\n';
	printDescription(manager.GetDeviceInfo(), cameraCount);
	// TODO: Print calibration data.
	printAuthority(manager.GetDongleKeyAuthority());
	std::cout << std::endl;
}

void cb_viewDataResult(void* callbackOwner, benano_scansdk::CViewDataResult* viewDataResult)
{
	std::cout << "[Info]Benano: Copying Point Cloud Data\n";
	benano_scansdk::CViewDataResult* dataCopy = new benano_scansdk::CViewDataResult;
	dataCopy->ScanSerialNumber = viewDataResult->ScanSerialNumber;
	if (viewDataResult->ZmapResult->Image2D) {
		dataCopy->ZmapResult = new benano_scansdk::CZmapData;
		dataCopy->ZmapResult->ChannelCountOfImage2D = viewDataResult->ZmapResult->ChannelCountOfImage2D;
		dataCopy->ZmapResult->ImageHeight = viewDataResult->ZmapResult->ImageHeight;
		dataCopy->ZmapResult->ImageWidth = viewDataResult->ZmapResult->ImageWidth;
		dataCopy->ZmapResult->ImageResolutionX = viewDataResult->ZmapResult->ImageResolutionX;
		dataCopy->ZmapResult->ImageResolutionY = viewDataResult->ZmapResult->ImageResolutionY;
		size_t imageSize = 3 * (size_t)dataCopy->ZmapResult->ImageWidth * (size_t)dataCopy->ZmapResult->ImageHeight;
		dataCopy->ZmapResult->Image2D = new benano_scansdk::U8[imageSize];
		dataCopy->ZmapResult->ImageRaw = new benano_scansdk::F32[imageSize];
		memcpy(dataCopy->ZmapResult->Image2D, viewDataResult->ZmapResult->Image2D, sizeof(benano_scansdk::U8) * imageSize);
		memcpy(dataCopy->ZmapResult->ImageRaw, viewDataResult->ZmapResult->ImageRaw, sizeof(benano_scansdk::F32) * imageSize);
	}

	if (viewDataResult->PointCloudResult.PointCloud) {
		dataCopy->PointCloudResult.PointsCount = viewDataResult->PointCloudResult.PointsCount;
		size_t cloudSize = 3 * (size_t)dataCopy->PointCloudResult.PointsCount;
		dataCopy->PointCloudResult.PointCloud = new benano_scansdk::F32[cloudSize];
		memcpy(dataCopy->PointCloudResult.PointCloud, viewDataResult->PointCloudResult.PointCloud, sizeof(benano_scansdk::F32) * cloudSize);
		if (viewDataResult->PointCloudResult.Normals != nullptr) {
			dataCopy->PointCloudResult.Normals = new benano_scansdk::F32[cloudSize];
			memcpy(dataCopy->PointCloudResult.Normals, viewDataResult->PointCloudResult.Normals, sizeof(benano_scansdk::F32) * cloudSize);
		}
	}

	if (viewDataResult->PointCloudResult.MeshVertices) {
		dataCopy->PointCloudResult.MeshCount = viewDataResult->PointCloudResult.MeshCount;
		size_t meshSize = 3 * (size_t)dataCopy->PointCloudResult.MeshCount;
		dataCopy->PointCloudResult.MeshVertices = new benano_scansdk::I32[meshSize];
		memcpy(dataCopy->PointCloudResult.MeshVertices, viewDataResult->PointCloudResult.MeshVertices, sizeof(benano_scansdk::I32) * meshSize);
	}

	*(benano_scansdk::CViewDataResult*)callbackOwner = *dataCopy;
	return;
}
