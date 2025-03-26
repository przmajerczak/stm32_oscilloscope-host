#include "DataRetriever.hpp"

#include "debug/Timemarker.hpp"
#include "sharedData/constants.hpp"
#include <fcntl.h>
#include <iostream>
#include <regex>
#include <termios.h>
#include <thread>

extern int errno;

void DataRetriever::runContinuousDataRetrieve(DynamicData &dynamicData)
{
    std::thread t([&]()
    {
        establishConnection();

        while (1)
        {
            singleDataRetrieve(dynamicData);
        }
    });
    t.detach();
}

void DataRetriever::establishConnection()
{
    deviceFileDescriptor = open(determineDeviceFilepath().c_str(), O_RDONLY);

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
}

std::string DataRetriever::determineDeviceFilepath()
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

void DataRetriever::singleDataRetrieve(DynamicData &dynamicData)
{
    Timemarker tmarker(
        dynamicData.timemarkersData.totalDataRetrievalAndDecodingDuration);

    constexpr uint16_t TWO_BYTES_PER_SAMPLE{2};
    constexpr uint16_t METADATA_BYTES{4};

    constexpr std::size_t expectedReceivedDataSize{
        TWO_BYTES_PER_SAMPLE * SAMPLES_PER_TRANSMISSION + METADATA_BYTES};

    EncodedAdcData undecodedRetrievedData{retrieveData(dynamicData)};
    std::size_t receivedBytes{undecodedRetrievedData.values.size()};

    while (receivedBytes != expectedReceivedDataSize)
    {
        std::cerr << "Received data transmission shorter than expected "
                  << expectedReceivedDataSize
                  << " bytes. Received bytes: " << receivedBytes << std::endl;

        undecodedRetrievedData = retrieveData(dynamicData);
        receivedBytes = undecodedRetrievedData.values.size();
    }

    if (undecodedRetrievedData.mode == DualChannelMode::ON)
    {
        dynamicData.active_channels.at(CHANNEL_1) = true;
        dynamicData.active_channels.at(CHANNEL_2) = true;
    }
    else
    {
        if (undecodedRetrievedData.channelId == CHANNEL_1)
        {
            dynamicData.active_channels.at(CHANNEL_1) = true;
            dynamicData.active_channels.at(CHANNEL_2) = false;
        }
        else
        {
            dynamicData.active_channels.at(CHANNEL_1) = false;
            dynamicData.active_channels.at(CHANNEL_2) = true;
        }

        dynamicData.trigger_source = undecodedRetrievedData.channelId;
    }

    dynamicData.frame_duration_ns =
        calculateFrameDuration_ns(undecodedRetrievedData.values);

    dynamicData.retrieved_adc_values.at(undecodedRetrievedData.channelId) =
        decodeAdcValues(undecodedRetrievedData.values);
}

EncodedAdcData DataRetriever::retrieveData(DynamicData &dynamicData)
{
    Timemarker tmarker(
        dynamicData.timemarkersData.singleFrameDataRetrievalDuration);

    uint8_t last_byte{0};
    uint8_t previous_byte{0};

    EncodedAdcValues values;

    constexpr uint8_t SECOND_LAST_BYTE_FOR_CHANNEL_1{0xff};
    constexpr uint8_t SECOND_LAST_BYTE_FOR_CHANNEL_2{0xfe};

    constexpr uint8_t ONE_CHANNEL_MODE{0xfe};
    constexpr uint8_t DUAL_CHANNEL_MODE{0xfd};

    constexpr uint8_t FAILED_READ_ATTEMPTS_LIMIT{10};
    uint8_t failed_read_attempts = 0;

    // TODO: wrap in nice method determining channel and return channel id along
    // with data
    while (
        not((last_byte == ONE_CHANNEL_MODE or last_byte == DUAL_CHANNEL_MODE) and
            (previous_byte == SECOND_LAST_BYTE_FOR_CHANNEL_1 or
             previous_byte == SECOND_LAST_BYTE_FOR_CHANNEL_2)))
    {
        previous_byte = last_byte;
        long int bytes_received{read(deviceFileDescriptor, &last_byte, 1)};
        if (bytes_received > 0)
        {
            values.push_back(last_byte);
        }
        else if (bytes_received == 0)
        {
            if (failed_read_attempts++ > FAILED_READ_ATTEMPTS_LIMIT)
            {
                std::cerr << "Device disconnected. Attemping reconnect. " << std::endl;

                close(deviceFileDescriptor);
                establishConnection();
            }
        }
    }

    values.pop_back();
    values.pop_back();

    ChannelId channelId{
        previous_byte == SECOND_LAST_BYTE_FOR_CHANNEL_1 ? CHANNEL_1 : CHANNEL_2};
    DualChannelMode mode{last_byte == DUAL_CHANNEL_MODE ? DualChannelMode::ON
                                                        : DualChannelMode::OFF};
    EncodedAdcData data{values, channelId, mode};

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

double DataRetriever::calculateFrameDuration_ns(
    EncodedAdcValues &undecodedRetrievedData)
{
    uint32_t timer_doubleticks_per_frame{
        pullFrameDurationFromUndecodedRetrievedData(undecodedRetrievedData)};

    constexpr double TIMER_COUNTS_UPWARDS_EDGE_TICKS{2.0};

    return timer_doubleticks_per_frame * DEVICE_TIMER_SINGLE_TICK_DURATION_NS *
           TIMER_COUNTS_UPWARDS_EDGE_TICKS;
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
