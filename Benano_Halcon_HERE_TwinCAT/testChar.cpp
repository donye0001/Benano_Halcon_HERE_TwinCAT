#include <iostream>


static void tckjgpo()
{
    
    char tx = 'TC:';
    int fullTime = 10;
    double coeffCopy[6];
    coeffCopy[0] = 1.2;
    coeffCopy[1] = 3.33;
    coeffCopy[2] = 10.7;
    coeffCopy[3] = 5.85;
    coeffCopy[4] = 3.654;
    coeffCopy[5] = 3.3333333333333;

    int size1 = sizeof(tx) + sizeof(fullTime) + sizeof(coeffCopy);
    std::cout << size1 << std::endl;
}