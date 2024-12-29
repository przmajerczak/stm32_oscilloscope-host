#pragma once

#include "DataAnalyzer.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <string>

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
    std::string retrieveData();
    int convertRawDataToValue(std::string raw_data);
    bool configureTty(const int deviceFileDescriptor);


    DataAnalyzer dataAnalyzer;
    int deviceFileDescriptor;
};
