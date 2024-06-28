#pragma once

#include "DataAnalyzer.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <optional>

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
    std::optional<std::string> retrieveData();
    int convertRawDataToValue(std::string raw_data);

    DataAnalyzer dataAnalyzer;
    int deviceFileDescriptor;
};
