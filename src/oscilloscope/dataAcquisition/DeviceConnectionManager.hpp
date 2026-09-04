#pragma once

#include <string>

class DeviceConnectionManager
{
public:
    DeviceConnectionManager(const bool test_mode_arg)
        : test_mode{test_mode_arg} {}
    int establishConnection();

private:
    std::string determineDeviceFilepath();
    bool configureTty(const int deviceFileDescriptor);

    const bool test_mode{false};
};
