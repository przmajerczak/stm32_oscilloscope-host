#include "DataRetriever.hpp"

#include <thread>
#include "utils.hpp"
#include <iostream>

extern int errno;

void DataRetriever::runContinuousDataRetrieve(const unsigned int timer_value_ms)
{
    std::thread t([timer_value_ms, this]()
    {
        while (1)
        {
            singleDataRetrieve();

            std::this_thread::sleep_for(std::chrono::milliseconds(timer_value_ms));
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
    deviceFileDescriptor = open("/dev/ttyACM0", O_RDONLY);

    if (deviceFileDescriptor > 0)
    {
        char rawData[4];
        int readBytes = read(deviceFileDescriptor, rawData, 4);

        close(deviceFileDescriptor);

        if (readBytes > 1)
        {
            std::string rawDataString{rawData};
            return rawDataString;
        }
        std::cerr << "No data received from the device." << std::endl;
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
