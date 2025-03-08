#include "DataRetriever.hpp"

#include "debug/Timemarker.hpp"
#include "sharedData/constants.hpp"
#include <fcntl.h>
#include <iostream>
#include <termios.h>
#include <thread>

extern int errno;

void DataRetriever::runContinuousDataRetrieve(DynamicData &dynamicData)
{
    std::thread t([&]()
    {
        // TODO: auto-detect correct path
        deviceFileDescriptor = open("/dev/ttyACM0", O_RDONLY);

        while (deviceFileDescriptor == -1)
        {
            std::cerr << "Error while trying to connect to the device. Retrying."
                      << std::endl;

            if (errno == EACCES) // permission denied
            {
                std::cerr << "Read access permission needed." << std::endl;
                system("sudo chmod +r /dev/ttyACM0");
            }

            deviceFileDescriptor = open("/dev/ttyACM0", O_RDONLY);
        }

        if (not configureTty(deviceFileDescriptor))
        {
            std::cerr << "Warning: tty configuration error. "
                         "Data decoding might be inaccurate." << std::endl;
        }

        while (1)
        {
            AdcValues retrieved_values{singleDataRetrieve(dynamicData)};
            dataAnalyzer.handleData(retrieved_values, dynamicData);
        }
    });
    t.detach();
}

AdcValues DataRetriever::singleDataRetrieve(DynamicData &dynamicData)
{
    Timemarker tmarker(dynamicData.timemarkersData.totalDataRetrievalAndDecodingDuration);

    constexpr std::size_t expectedReceivedDataSize{24004};

    EncodedAdcValues undecodedRetrievedData{retrieveData(dynamicData)};
    std::size_t receivedBytes{undecodedRetrievedData.size()};

    while (receivedBytes != expectedReceivedDataSize)
    {
        std::cerr << "Received data transmission shorter than expected "
                  << expectedReceivedDataSize
                  << " bytes. Received bytes: " << receivedBytes << std::endl;

        undecodedRetrievedData = retrieveData(dynamicData);
        receivedBytes = undecodedRetrievedData.size();
    }

    dynamicData.frame_duration_ns =
        calculateFrameDuration_ns(undecodedRetrievedData);

    return decodeAdcValues(undecodedRetrievedData);
}

EncodedAdcValues DataRetriever::retrieveData(DynamicData &dynamicData)
{
    Timemarker tmarker(dynamicData.timemarkersData.singleFrameDataRetrievalDuration);

    uint8_t byte{0};
    uint8_t previous_byte{0};

    EncodedAdcValues data;

    while (not(byte == 0xff and previous_byte == 0xff))
    {
        previous_byte = byte;
        long int bytes_received{read(deviceFileDescriptor, &byte, 1)};
        if (bytes_received > 0)
        {
            data.push_back(byte);
        }
    }

    data.pop_back();
    data.pop_back();

    return data;
}

AdcValues
DataRetriever::decodeAdcValues(const EncodedAdcValues &encoded_values)
{
    AdcValues decoded_values;
    decoded_values.resize(encoded_values.size() / 2);

    auto current_encoded_values{encoded_values.begin()};
    auto next_encoded_values{std::next(current_encoded_values, 1)};

    for (auto &decoded_value : decoded_values)
    {
        decoded_value = *current_encoded_values + (*next_encoded_values << 8);

        ++current_encoded_values;
        ++current_encoded_values;
        ++next_encoded_values;
        ++next_encoded_values;
    }

    return decoded_values;
}

bool DataRetriever::configureTty(const int deviceFileDescriptor)
{
    struct termios ttySettings;
    if (tcgetattr(deviceFileDescriptor, &ttySettings) != 0)
    {
        std::cerr << "Error: could not get current terminal settings." << std::endl;
        return false;
    }

    cfmakeraw(&ttySettings);
    ttySettings.c_lflag &= ~ICANON;
    ttySettings.c_lflag &= ~ECHO;
    ttySettings.c_lflag &= ~ECHOE;
    ttySettings.c_lflag &= ~ISIG;
    ttySettings.c_oflag &= ~OPOST;

    if (tcsetattr(deviceFileDescriptor, TCSANOW, &ttySettings) != 0)
    {
        std::cerr << "Error: could not set terminal settings." << std::endl;
        return false;
    }

    return true;
}

double DataRetriever::calculateFrameDuration_ns(EncodedAdcValues &undecodedRetrievedData)
{
    uint32_t timer_doubleticks_per_frame{
        pullFrameDurationFromUndecodedRetrievedData(undecodedRetrievedData)};

    constexpr double TIMER_COUNTS_UPWARDS_EDGE_TICKS{2.0};

    return timer_doubleticks_per_frame * DEVICE_TIMER_SINGLE_TICK_DURATION_NS * TIMER_COUNTS_UPWARDS_EDGE_TICKS;
}

uint32_t DataRetriever::pullFrameDurationFromUndecodedRetrievedData(
    EncodedAdcValues &undecodedRetrievedData)
{
    uint32_t timer_doubleticks_per_frame{0};

    for (int i = 3; i >= 0; --i)
    {
        timer_doubleticks_per_frame += (undecodedRetrievedData.back() << (i * 8));
        undecodedRetrievedData.pop_back();
    }

    return timer_doubleticks_per_frame;
}
