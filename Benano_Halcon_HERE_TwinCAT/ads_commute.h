#pragma once
#include <iostream>
#include <regex>
#include <Windows.h>

#include "ads.h"
#include "ads_coordinate.h"

#include <TcAdsDef.h>
#include <TcAdsAPI.h>

namespace ads
{
    class AdsCommute
    {
    public:
        AdsCommute();
        ~AdsCommute();

        long call(unsigned long idxGroup, unsigned long idxOffset, Linear linear);
        bool close();
        bool isOpened();
        bool open();
        long read(unsigned long idxGroup, unsigned long idxOffset, Linear& linear);///////////////////////////////improtant wrt in report
        bool setTargetAddress(unsigned short port, const char* netID = "local");
        long write(unsigned long idxGroup, unsigned long idxOffset, Linear linear);

    private:
        AmsAddr localAddr, targetAddr;
        long adsError;
        bool portOpened, targetIsSet;

        bool detectNetID(AmsNetId& netID, std::string context);
        void printAddress(const AmsAddr& address, const char* indent = "");
        void printNetID(const AmsNetId& netID, const char* prefix = "");
    };
} /* ads */