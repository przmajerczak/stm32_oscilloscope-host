#pragma once

#include "DataAnalyzer.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <optional>

class DataRetriever
{
public:
    void runContinuousDataRetrieve(const unsigned int timer_value_ms);

private:
    void singleDataRetrieve();
    std::optional<std::string> retrieveData();
    int convertRawDataToValue(std::string raw_data);

    DataAnalyzer dataAnalyzer;
    int deviceFileDescriptor;
};
