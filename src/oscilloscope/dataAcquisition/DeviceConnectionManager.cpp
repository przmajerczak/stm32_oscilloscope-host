#include "DeviceConnectionManager.hpp"

#include <fcntl.h>
#include <linux/limits.h>
#include <termios.h>
#include <unistd.h>

#include <cstdint>
#include <iostream>
#include <regex>

#include "test/DummyOutputGenerator.hpp"

extern int errno;

int DeviceConnectionManager::establishConnection()
{
    if (test_mode)
    {
        DummyOutputGenerator{}.generate();

        constexpr const char *filepath_tmp{"dummy_output"};
        const int deviceFileDescriptor = open(filepath_tmp, O_RDONLY);

        return deviceFileDescriptor;
    }

    int deviceFileDescriptor = open(determineDeviceFilepath().c_str(), O_RDONLY);

    constexpr uint8_t FAILED_CONNECTION_ATTEMPTS_LOGGING_LIMIT{20};
    uint8_t failed_connection_attempts_since_last_log =
        FAILED_CONNECTION_ATTEMPTS_LOGGING_LIMIT + 1;

    while (deviceFileDescriptor == -1)
    {
        if (errno == EACCES) // permission denied
        {
            const std::string device_filepath{determineDeviceFilepath()};
            std::cerr << "Read access permission needed for oscilloscope serial "
                         "device file "
                      << device_filepath << std::endl;

            std::string chmod_command{"sudo chmod +r "};
            chmod_command.append(device_filepath);

            system(chmod_command.c_str());
        }
        else
        {
            if (failed_connection_attempts_since_last_log++ >
                FAILED_CONNECTION_ATTEMPTS_LOGGING_LIMIT)
            {
                std::cerr << "Error while trying to connect to the device. Retrying."
                          << std::endl;

                failed_connection_attempts_since_last_log = 0;
            }
        }

        deviceFileDescriptor = open(determineDeviceFilepath().c_str(), O_RDONLY);
    }

    if (not configureTty(deviceFileDescriptor))
    {
        std::cerr << "Warning: tty configuration error. "
                     "Data decoding might be inaccurate."
                  << std::endl;
    }

    return deviceFileDescriptor;
}

std::string DeviceConnectionManager::determineDeviceFilepath()
{
    FILE *command_results;
    char list_of_serial_devices[PATH_MAX];
    std::string filename{"ttyACM0"};

    command_results = popen("ls -l /dev/serial/by-id 2> /dev/null", "r");
    if (command_results != nullptr)
    {
        while (fgets(list_of_serial_devices, PATH_MAX, command_results) !=
               nullptr)
        {
            std::string device_info{list_of_serial_devices};

            std::regex device_regex("STM32_Virtual_ComPort");
            std::smatch regex_matches;
            std::regex_search(device_info, regex_matches, device_regex);

            if (not regex_matches.empty())
            {
                std::regex device_filename_regex("tty\\w+");
                std::regex_search(device_info, regex_matches, device_filename_regex);
                if (regex_matches.size() == 1)
                {
                    filename = regex_matches.str();
                    break;
                }
            }
        }
    }
    else
    {
        std::cerr << "Failed to find device file path. Using default one."
                  << std::endl;
    }

    pclose(command_results);

    std::string filepath{"/dev/"};
    filepath.append(filename);

    return filepath;
}

bool DeviceConnectionManager::configureTty(const int deviceFileDescriptor)
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
