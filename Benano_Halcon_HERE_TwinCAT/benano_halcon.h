#pragma once
#include <Windows.h>
#include <HalconCpp.h>
#include "benano_init.h"

void benanoStuff(HalconCpp::HTuple& socketData, HalconCpp::HTuple command, benano_scansdk::CScanManager& manager)
{
	std::string format;
	HalconCpp::HTuple To, data;
	benano_scansdk::CViewDataResult scanData;
	benano_scansdk::CScanResultCallback rltCallback;
	benano_scansdk::eStartScanStatus status;
	benano_scansdk::eScanStatus otherStatus, lastStatus;
	rltCallback.SetViewDataResultCallback(&scanData, cb_viewDataResult);
	int cmd = command.I();
	switch (cmd)
	{
	case 1000:
		// Not implemented
		HalconCpp::SendData(socketData, "z", "Not implemented", To);
		break;
	case 1001:
		HalconCpp::SendData(socketData, "z", "Not implemented", To);
		// Not implemented
		break;
	case 1002:
		HalconCpp::SendData(socketData, "z", "Not implemented", To);
		// Not implemented
		break;
	case 1003:  // Scan
		std::cout << "[Info]Command: Scan\n";
		status = manager.GetScanController().StartScan(rltCallback);
		switch (status)
		{
		case benano_scansdk::eStartScanStatus::StartScan_NoError:
			lastStatus = manager.GetScanController().GetScanStatus();
			while (manager.GetScanController().IsScanStillRunning()) {
				otherStatus = manager.GetScanController().GetScanStatus();
				if (otherStatus != lastStatus) {
					lastStatus = otherStatus;
					std::cout << "[Info]Scan Status: ";
					switch (otherStatus) {
					case benano_scansdk::eScanStatus::ePrepareScanFlow:
						std::cout << "Prepare Scan Flow\n";
						break;
					case benano_scansdk::eScanStatus::eScanIdle:
						std::cout << "Scan Idle\n";
						break;
					case benano_scansdk::eScanStatus::eWaitCamImages:
						std::cout << "Wait Images\n";
						break;
					case benano_scansdk::eScanStatus::eWaitPublishDone:
						std::cout << "Wait Publish\n";
						break;
					case benano_scansdk::eScanStatus::eWaitSolverDone:
						std::cout << "Wait Solver Done\n";
						break;
					}
				}
				Sleep(1000);
			}
			std::cout << "[Info]Stop Scan\n";
			manager.GetScanController().StopScan();
			if (!scanData.PointCloudResult.PointCloud) {
				std::cerr << "[Error]Benano: Can't get point cloud data\n\n";
				HalconCpp::SendData(socketData, "z", "Failed", To);
				return;
			}

			std::cout << "[Info]Send Data\n";
			data.Append("Success");
			data.Append((Hlong)scanData.PointCloudResult.PointsCount);
			HalconCpp::SendData(socketData, "zq", data, To);
			data.Clear();
			data.Append(HalconCpp::HTuple(scanData.PointCloudResult.PointCloud, 3 * scanData.PointCloudResult.PointsCount));
			format = "f";
			format.append(std::to_string(3 * scanData.PointCloudResult.PointsCount));
			HalconCpp::SendData(socketData, format.c_str(), data, To);
			delete scanData.PointCloudResult.Colors;
			delete scanData.PointCloudResult.PointCloud;
			delete scanData.PointCloudResult.MeshVertices;
			delete scanData.ZmapResult;
			scanData.CameraResult.clear();
			break;
		case benano_scansdk::eStartScanStatus::StartScan_NoConnectedDevice:
			HalconCpp::SendData(socketData, "z", "No connection", To);
			break;
		case benano_scansdk::eStartScanStatus::StartScan_ScanStillRunning:
			HalconCpp::SendData(socketData, "z", "Wait", To);
			break;
		}
	}

}
