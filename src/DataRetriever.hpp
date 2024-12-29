#pragma once

#include "DataAnalyzer.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <vector>

class DataRetriever
{
public:
    ~DataRetriever()
    {
        close(deviceFileDescriptor);
    }

    void runContinuousDataRetrieve();

private:
    void singleDataRetrieve();
    std::vector<uint8_t>  retrieveData();
    std::vector<uint16_t> convertRawDataToValues(std::vector<uint8_t> raw_data);
    bool configureTty(const int deviceFileDescriptor);

    DataAnalyzer dataAnalyzer;
    int deviceFileDescriptor;
};
