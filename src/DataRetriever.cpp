#include "DataRetriever.hpp"

#include <thread>
#include "utils.hpp"
#include <iostream>

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
    if (device.is_open())
    {
        std::string rawData;

        for (unsigned int attempt_count = 0; attempt_count < FILE_READ_ATTEMPTS_LIMIT; ++attempt_count)
        {
            std::getline(device, rawData);
            if (not rawData.empty())
            {
                return rawData;
            }
        }
        std::cerr << "No data received from the device." << std::endl;
    }
    else
    {
        std::cerr << "Error: unable to connect to the device. Retrying." << std::endl;
        device.open("/dev/ttyACM0");
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
