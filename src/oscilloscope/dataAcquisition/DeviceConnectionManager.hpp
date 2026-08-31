#pragma once

#include <string>

class DeviceConnectionManager
{
public:
    int establishConnection();

private:
    std::string determineDeviceFilepath();
    bool configureTty(const int deviceFileDescriptor);
};
