//#include "ads_commute.h"
//
//
//ads::AdsCommute::AdsCommute()
//{
//    adsError = 0;
//    localAddr.netId = { 192, 168, 25, 1, 1, 1 };
//    localAddr.port = 851;
//    targetAddr = localAddr;
//    portOpened = false;
//    targetIsSet = false;
//    open();
//}
//
//ads::AdsCommute::~AdsCommute()
//{
//    close();
//}
//
//long ads::AdsCommute::call(unsigned long idxGroup, unsigned long idxOffset, Global Global)
//{
//    return AdsSyncReadReq(&targetAddr, idxGroup, idxOffset,
//        sizeof(Global.req), &Global.req);
//}
//
//bool ads::AdsCommute::close()
//{
//    if (!portOpened) {
//        return true;
//    }
//
//    std::cout << "Close ADS port: " << localAddr.port << '\n';
//    adsError = AdsPortClose();
//    if (adsError) {
//        std::cerr << "Error: ADS port close return: " << adsError << std::endl;
//        return false;
//    }
//
//    portOpened = false;
//    return true;
//}
//
//bool ads::AdsCommute::detectNetID(AmsNetId& netID, std::string context)
//{
//    std::smatch matches;
//
//    // Grab whole AMS net ID
//    std::regex reg("\\b((25[0-5]|2[0-4]\\d|[01]?\\d?\\d)\\.){5}(25[0-5]|2[0-4]\\d|[01]?\\d?\\d)\\b");
//    std::regex_search(context, matches, reg);
//    context = matches.str();
//    if (context.empty()) {
//        return false;
//    }
//
//    // Grab each value
//    unsigned char ID[6];
//    int count = 0;
//    std::regex reg1("\\b(25[0-5]|2[0-4]\\d|[01]?\\d?\\d)\\b");
//    while (std::regex_search(context, matches, reg1)) {
//        if (count == 6) {
//            return false;
//        }
//
//        ID[count++] = (unsigned char)std::stoi(matches.str());
//        context = matches.suffix().str();
//    }
//
//    if (count != 6) {
//        return false;
//    }
//
//    for (int i = 0; i < 6; i++) {
//        netID.b[i] = ID[i];
//    }
//
//    return true;
//}
//
//bool ads::AdsCommute::isOpened()
//{
//    return portOpened;
//}
//
//bool ads::AdsCommute::open()
//{
//    if (portOpened) {
//        return true;
//    }
//
//    std::cout << "Open ADS port: " << AdsPortOpen() << '\n';
//    adsError = AdsGetLocalAddress(&localAddr);
//    if (adsError) {
//        portOpened = false;
//        std::cerr << "Error: ADS get local address return: " << adsError << std::endl;
//        return false;
//    }
//
//    std::cout << "Local ADS address:\n";
//    printAddress(localAddr, "  ");
//    portOpened = true;
//
//    return true;
//}
//
//void ads::AdsCommute::printAddress(const AmsAddr& address, const char* indent)
//{
//    printNetID(address.netId, indent);
//    std::cout << indent << "Port: " << address.port << std::endl;
//}
//
//void ads::AdsCommute::printNetID(const AmsNetId& netID, const char* prefix)
//{
//    std::cout << prefix << "NetID: ";
//    for (char i = 0; i < 6; i++) {
//        std::cout << (int)netID.b[i] << '.';
//    }
//    std::cout << "\b " << std::endl;
//}
//
//
//
//bool ads::AdsCommute::setTargetAddress(unsigned short port, const char* netID)
//{
//    if (!portOpened) {
//        return false;
//    }
//
//    if (strlen(netID) > 23) {
//        return false;
//    }
//
//    if (port == 0) {
//        return false;
//    }
//
//    if (netID == "local") {
//        targetAddr.netId = localAddr.netId;
//    }
//    else {
//        if (!detectNetID(targetAddr.netId, netID)) {
//            return false;
//        }
//    }
//
//    targetAddr.port = port;
//    targetIsSet = true;
//    return true;
//}
//
//long ads::AdsCommute::write(unsigned long idxGroup, unsigned long idxOffset, Global Global)
//{
//    return AdsSyncWriteReq(&targetAddr, idxGroup, idxOffset,
//        sizeof(Global.req), &Global.req);
//}
