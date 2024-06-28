#include "DataRetriever.hpp"

#include <thread>
#include "utils.hpp"
#include <iostream>

extern int errno;

void DataRetriever::runContinuousDataRetrieve()
{
    std::thread t([this]()
    {
        deviceFileDescriptor = open("/dev/ttyACM0", O_RDONLY);

        while (1)
        {
            singleDataRetrieve();
        }
    });
    t.detach();
}

void DataRetriever::singleDataRetrieve()
{
    auto raw_data{retrieveData()};

    if (raw_data.has_value())
    {
        int retrieved_value{convertRawDataToValue(raw_data.value())};
        dataAnalyzer.handleData(retrieved_value);
    }
}

std::optional<std::string> DataRetriever::retrieveData()
{
    if (deviceFileDescriptor > 0)
    {
        char rawData[4];

        int readBytes = read(deviceFileDescriptor, rawData, 4);

        while (readBytes != 4)
        {
            readBytes = read(deviceFileDescriptor, rawData, 4);
        }

        std::string rawDataString{rawData};
        return rawDataString;
    }
    else
    {
        if (errno == EACCES) // permission denied
        {
            system("sudo chmod +r /dev/ttyACM0");
        }
        else
        {
            std::cerr << "Error: unable to connect to the device. Retrying." << std::endl;

            deviceFileDescriptor = open("/dev/ttyACM0", O_RDONLY);
        }
    }
    return {};
}

int DataRetriever::convertRawDataToValue(std::string raw_data)
{
    int value{0};

    try
    {
        value = std::stoi(raw_data);
    }
    catch (...)
    {
        std::cerr << "Stoi failed for raw data: \"" << raw_data << "\"." << std::endl;
    }

    return value;
}
