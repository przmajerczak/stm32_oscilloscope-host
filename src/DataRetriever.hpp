#pragma once

#include "DataAnalyzer.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <list>

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
    std::list<uint8_t> retrieveData();
    RawValuesContainer convertRawDataToValues(std::list<uint8_t> raw_data);
    bool configureTty(const int deviceFileDescriptor);

    DataAnalyzer dataAnalyzer;
    int deviceFileDescriptor;
};
