#include "DataRetriever.hpp"

#include <thread>
#include "utils.hpp"
#include <iostream>

extern int errno;

void DataRetriever::runContinuousDataRetrieve()
{
    std::thread t([this]()
    {
        // TODO: auto-detect correct path
        deviceFileDescriptor = open("/dev/ttyACM0", O_RDONLY);

        while (deviceFileDescriptor == 0)
        {
            std::cerr << "Error while trying to connect to the device. Retrying." << std::endl;

            if (errno == EACCES) // permission denied
            {
                std::cerr << "Read access permission needed." << std::endl;
                system("sudo chmod +r /dev/ttyACM0");
            }

            deviceFileDescriptor = open("/dev/ttyACM0", O_RDONLY);
        }

        while (1)
        {
            singleDataRetrieve();
        }
    });
    t.detach();
}

void DataRetriever::singleDataRetrieve()
{
    int retrieved_value{convertRawDataToValue(retrieveData())};
    dataAnalyzer.handleData(retrieved_value);
}

std::string DataRetriever::retrieveData()
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
