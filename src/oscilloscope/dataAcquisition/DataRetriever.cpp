#include "DataRetriever.hpp"

#include "debug/Timemarker.hpp"
#include "sharedData/constants.hpp"
#include <fcntl.h>
#include <iostream>
#include <thread>
#include <unistd.h>

DataRetriever::~DataRetriever() { close(deviceFileDescriptor); }

void DataRetriever::runContinuousDataRetrieve(DynamicData &dynamicData)
{
    std::thread t([&]()
    {
        deviceFileDescriptor = device.establishConnection();

        while (1)
        {
            singleDataRetrieve(dynamicData);
            dynamicData.new_data_available = true;
        }
    });
    t.detach();
}

AdcValuesArray DataRetriever::getCopyOfRetrievedAdcValues(const ChannelId chId)
{
    // TODO: handle multithreading better
    return retrieved_adc_values.at(chId);
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

    retrieved_adc_values.at(undecodedRetrievedData.channelId) =
        decodeAdcValues(undecodedRetrievedData.values);
}

EncodedAdcData DataRetriever::retrieveData(DynamicData &dynamicData)
{
    Timemarker tmarker(
        dynamicData.timemarkersData.singleFrameDataRetrievalDuration);

    uint8_t last_byte{0};
    uint8_t previous_byte{0};

    EncodedAdcValues values;

    constexpr uint8_t FAILED_READ_ATTEMPTS_LIMIT{10};
    uint8_t failed_read_attempts = 0;

    while (not(detectEndSequence(previous_byte, last_byte)))
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
                deviceFileDescriptor = device.establishConnection();
            }
        }
    }

    values.pop_back();
    values.pop_back();

    EncodedAdcData data{values, determineChannelId(previous_byte),
                        determineChannelMode(last_byte)};

    return data;
}

bool DataRetriever::detectEndSequence(const uint8_t second_last_byte,
                                      const uint8_t last_byte) const
{
    return (determineChannelMode(last_byte) != DualChannelMode::INVALID) and
           (determineChannelId(second_last_byte) != NUMBER_OF_CHANNELS);
}

DualChannelMode
DataRetriever::determineChannelMode(const uint8_t last_byte) const
{
    constexpr uint8_t LAST_BYTE_FOR_ONE_CHANNEL_MODE{0xfe};
    constexpr uint8_t LAST_BYTE_FOR_DUAL_CHANNEL_MODE{0xfd};

    if (last_byte == LAST_BYTE_FOR_ONE_CHANNEL_MODE)
    {
        return DualChannelMode::OFF;
    }

    if (last_byte == LAST_BYTE_FOR_DUAL_CHANNEL_MODE)
    {
        return DualChannelMode::ON;
    }

    return DualChannelMode::INVALID;
}

ChannelId
DataRetriever::determineChannelId(const uint8_t second_last_byte) const
{
    constexpr uint8_t SECOND_LAST_BYTE_FOR_CHANNEL_1{0xff};
    constexpr uint8_t SECOND_LAST_BYTE_FOR_CHANNEL_2{0xfe};

    if (second_last_byte == SECOND_LAST_BYTE_FOR_CHANNEL_1)
    {
        return CHANNEL_1;
    }

    if (second_last_byte == SECOND_LAST_BYTE_FOR_CHANNEL_2)
    {
        return CHANNEL_2;
    }

    return NUMBER_OF_CHANNELS;
}

AdcValuesArray DataRetriever::decodeAdcValues(const EncodedAdcValues &encoded_values)
{
    AdcValuesArray decoded_values;

    auto current_encoded_values{encoded_values.begin()};
    auto next_encoded_values{std::next(current_encoded_values, 1)};

    for (std::size_t i = 0; i < SAMPLES_PER_TRANSMISSION; ++i)
    {
        decoded_values.at(i) = *current_encoded_values + (*next_encoded_values << 8);

        ++current_encoded_values;
        ++current_encoded_values;
        ++next_encoded_values;
        ++next_encoded_values;
    }

    return decoded_values;
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
