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
