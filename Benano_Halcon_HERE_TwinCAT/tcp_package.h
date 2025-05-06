#pragma once
#include <string>
#include <vector>

#pragma pack(push, 1) // �קK�J��padding�����D

// Client 1: TwinCAT rx
struct DataType10
{
    char rx; // ����
    int isTouch;
    int isTake;
    double fullTime;
    double coeffCopy[6][6];
};

// Client 1: TwinCAT tx
struct DataType11
{
    char tx; // ����
    int moveMode;
    double position[6];
};

// Client 2: Gazebo rx
struct DataType20
{
    char rx;
    bool isOK;
};

// Client 2: Gazebo tx
struct DataType21
{
    char tx;
    int isTouch;
    int isTake;
    double fullTime;
    double coeffCopy[6][6];
};

// Client 3: int, bool, double
struct DataType3 {
    char identifier = 'C';
    int value1;
    bool value2;
    double value3;
};

// Client 4: string, int
struct DataType4 {
    char identifier = 'D';
    std::string value1;
    int value2;
};
#pragma pack(pop) 
