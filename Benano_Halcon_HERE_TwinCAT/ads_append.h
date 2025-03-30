#pragma once
#include <HalconCpp.h>
#include <TcAdsDef.h>
#include <TcAdsAPI.h>

#include <iostream>
#include <stdexcept>


//void twincatStuff(HalconCpp::HTuple& socket, int command)
//{
//
//}


//#pragma once
//
//#include <HalconCpp.h>
//#include <TcAdsDef.h>
//#include <TcAdsAPI.h>
//
//#include <iostream>
//#include <stdexcept>
//#include <array>
//
//namespace ads {
//
//    
//
//    class AdsCommute {
//    public:
//        AdsCommute();
//        ~AdsCommute();
//
//        bool open();
//        bool close();
//        bool isOpened();
//
//        bool setTargetAddress(unsigned short port, const char* netID);
//        long writeCoordinates(unsigned long idxGroup, unsigned long idxOffset, const Coordinates& coordinates);
//        long readCoordinates(unsigned long idxGroup, unsigned long idxOffset, Coordinates& coordinates);
//
//        void printAddress(const AmsAddr& address, const char* indent = "");
//        void printNetID(const AmsNetId& netID, const char* prefix = "");
//
//        bool detectNetID(AmsNetId& netID, const std::string& context);
//
//        // Custom function to send Halcon coordinates to TwinCAT
//        void sendHalconCoordinates(HalconCpp::HTuple& halconData, unsigned long idxGroup, unsigned long idxOffset);
//
//    private:
//        AmsAddr localAddr;
//        AmsAddr targetAddr;
//        long adsError;
//        bool portOpened;
//        bool targetIsSet;
//    };
//
//} // namespace ads
//
//// Implementation
//
//namespace ads {
//
//    AdsCommute::AdsCommute()
//        : adsError(0), portOpened(false), targetIsSet(false) {
//        localAddr.netId = { 0, 0, 0, 0, 0, 0 };
//        localAddr.port = 851;
//        targetAddr = localAddr;
//        open();
//    }
//
//    AdsCommute::~AdsCommute() {
//        close();
//    }
//
//    bool AdsCommute::open() {
//        if (portOpened) return true;
//
//        std::cout << "Opening ADS port..." << std::endl;
//        adsError = AdsPortOpen();
//        if (adsError) {
//            std::cerr << "Error: Failed to open ADS port, code: " << adsError << std::endl;
//            return false;
//        }
//
//        adsError = AdsGetLocalAddress(&localAddr);
//        if (adsError) {
//            std::cerr << "Error: Failed to get local address, code: " << adsError << std::endl;
//            return false;
//        }
//
//        portOpened = true;
//        printAddress(localAddr, "Local ");
//        return true;
//    }
//
//    bool AdsCommute::close() {
//        if (!portOpened) return true;
//
//        std::cout << "Closing ADS port..." << std::endl;
//        adsError = AdsPortClose();
//        if (adsError) {
//            std::cerr << "Error: Failed to close ADS port, code: " << adsError << std::endl;
//            return false;
//        }
//
//        portOpened = false;
//        return true;
//    }
//
//    bool AdsCommute::isOpened() {
//        return portOpened;
//    }
//
//    bool AdsCommute::setTargetAddress(unsigned short port, const char* netID) {
//        if (!portOpened || port == 0 || strlen(netID) > 23) return false;
//
//        if (std::string(netID) == "local") {
//            targetAddr.netId = localAddr.netId;
//        }
//        else if (!detectNetID(targetAddr.netId, netID)) {
//            return false;
//        }
//
//        targetAddr.port = port;
//        targetIsSet = true;
//        return true;
//    }
//
//    long AdsCommute::writeCoordinates(unsigned long idxGroup, unsigned long idxOffset, const Coordinates& coordinates) {
//        // Write the coordinates to the target address in ADS
//        return AdsSyncWriteReq(&targetAddr, idxGroup, idxOffset, sizeof(coordinates), &coordinates);
//    }
//
//    long AdsCommute::readCoordinates(unsigned long idxGroup, unsigned long idxOffset, Coordinates& coordinates) {
//        // Read the coordinates from the target address in ADS
//        return AdsSyncReadReq(&targetAddr, idxGroup, idxOffset, sizeof(coordinates), &coordinates);
//    }
//
//    bool AdsCommute::detectNetID(AmsNetId& netID, const std::string& context) {
//        std::regex reg(R"(((25[0-5]|2[0-4]\d|[01]?\d?\d)\.){5}(25[0-5]|2[0-4]\d|[01]?\d?\d))");
//        std::smatch match;
//        if (!std::regex_search(context, match, reg)) return false;
//
//        unsigned char IDs[6];
//        std::istringstream iss(match.str());
//        for (int i = 0; i < 6; ++i) {
//            int value;
//            if (!(iss >> value) || value < 0 || value > 255) return false;
//            IDs[i] = static_cast<unsigned char>(value);
//        }
//        std::copy(IDs, IDs + 6, netID.b);
//        return true;
//    }
//
//    void AdsCommute::printAddress(const AmsAddr& address, const char* indent) {
//        printNetID(address.netId, indent);
//        std::cout << indent << "Port: " << address.port << std::endl;
//    }
//
//    void AdsCommute::printNetID(const AmsNetId& netID, const char* prefix) {
//        std::cout << prefix << "NetID: ";
//        for (int i = 0; i < 6; ++i) {
//            std::cout << static_cast<int>(netID.b[i]) << (i < 5 ? '.' : ' ');
//        }
//        std::cout << std::endl;
//    }
//
//    void AdsCommute::sendHalconData(HalconCpp::HTuple& halconData, unsigned long idxGroup, unsigned long idxOffset) {
//        if (!targetIsSet) {
//            throw std::runtime_error("Target address is not set.");
//        }
//
//        if (halconData.Length() < 3) {
//            throw std::runtime_error("Insufficient data in Halcon tuple. At least X, Y, Z required.");
//        }
//
//        Coordinates coordinates = { 0, 0, 0, 0, 0, 0 };
//        coordinates.x = halconData[0].D();  // X
//        coordinates.y = halconData[1].D();  // Y
//        coordinates.z = halconData[2].D();  // Z
//
//        // Optional Rx, Ry, Rz
//        if (halconData.Length() >= 6) {
//            coordinates.rx = halconData[3].D();
//            coordinates.ry = halconData[4].D();
//            coordinates.rz = halconData[5].D();
//        }
//
//        long result = writeCoordinates(idxGroup, idxOffset, coordinates);
//        if (result) {
//            std::cerr << "Error: Failed to write coordinates to ADS target, code: " << result << std::endl;
//        }
//    }
//
//} // namespace ads

