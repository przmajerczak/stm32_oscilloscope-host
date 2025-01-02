#include "DataRetriever.hpp"

#include <thread>
#include "utils.hpp"
#include <iostream>
#include <termios.h>

extern int errno;

void DataRetriever::runContinuousDataRetrieve()
{
    std::thread t([this]()
    {
        // TODO: auto-detect correct path
        deviceFileDescriptor = open("/dev/ttyACM0", O_RDONLY);

        while (deviceFileDescriptor == -1)
        {
            std::cerr << "Error while trying to connect to the device. Retrying." << std::endl;

            if (errno == EACCES) // permission denied
            {
                std::cerr << "Read access permission needed." << std::endl;
                system("sudo chmod +r /dev/ttyACM0");
            }

            deviceFileDescriptor = open("/dev/ttyACM0", O_RDONLY);
        }

        if (not configureTty(deviceFileDescriptor))
        {
            std::cerr << "Warning: tty configuration error. Data decoding might be inaccurate." << std::endl;
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
    auto undecodedRetrievedData{retrieveData()};
    constexpr std::size_t expectedReceivedDataSizeUnderInterfaceV2_0{1604};
    const std::size_t receivedBytes{undecodedRetrievedData.size()};

    // TODO: fix shattered transmissions issue
    if (receivedBytes != expectedReceivedDataSizeUnderInterfaceV2_0)
    {
        std::cerr << "Received data transmission shorter than expected "
                  << expectedReceivedDataSizeUnderInterfaceV2_0
                  << ". Received bytes: " << receivedBytes << std::endl;

        return;
    }

    [[maybe__unused]] uint32_t measurement_period{
        pullMeasurementPeriodFromUndecodedRetrievedData(undecodedRetrievedData)};

    RawValuesContainer retrieved_values{
        convertRawDataToValues(undecodedRetrievedData)};
    dataAnalyzer.handleData(retrieved_values);
}

std::list<uint8_t> DataRetriever::retrieveData()
{
    uint8_t byte{0};
    uint8_t previous_byte{0};

    std::list<uint8_t> data;

    while (not(byte == 0xff and previous_byte == '\n'))
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

RawValuesContainer DataRetriever::convertRawDataToValues(std::list<uint8_t> raw_data)
{
    RawValuesContainer values;
    values.resize(raw_data.size() / 2);

    auto values_it{values.begin()};

    auto current_raw_data{raw_data.begin()};
    auto next_raw_data{std::next(current_raw_data, 1)};

    for (auto &value : values)
    {
        value = *current_raw_data + (*next_raw_data << 8);

        ++current_raw_data;
        ++current_raw_data;
        ++next_raw_data;
        ++next_raw_data;
    }

    return values;
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

uint32_t DataRetriever::pullMeasurementPeriodFromUndecodedRetrievedData(
    std::list<uint8_t> &undecodedRetrievedData)
{

    uint32_t measurement_period{0};

    for (int i = 3; i >= 0; --i)
    {
        measurement_period += (undecodedRetrievedData.back() << (i * 8));
        undecodedRetrievedData.pop_back();
    }

    std::cout << "Measurement period: "
              << static_cast<long long int>(measurement_period) << std::endl;

    return measurement_period;
}
