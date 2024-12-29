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
    std::vector<uint16_t> retrieved_values{convertRawDataToValues(retrieveData())};
    dataAnalyzer.handleData(retrieved_values);
}

std::vector<uint8_t> DataRetriever::retrieveData()
{
    uint8_t byte{0};
    uint8_t previous_byte{0};

    std::vector<uint8_t> data;

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

std::vector<uint16_t> DataRetriever::convertRawDataToValues(std::vector<uint8_t> raw_data)
{
    std::vector<uint16_t> values;
    values.resize(raw_data.size() / 2);

    auto values_it{values.begin()};

    for (std::size_t i = 0; i < raw_data.size(); i += 2)
    {
        *values_it = raw_data.at(i) + (raw_data.at(i + 1) << 8);

        ++values_it;
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
