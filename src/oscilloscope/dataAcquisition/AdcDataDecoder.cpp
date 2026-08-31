#include "AdcDataDecoder.hpp"

#include "sharedData/constants.hpp"

AdcValuesArray AdcDataDecoder::decodeAdcValues(const EncodedAdcValues &encoded_values)
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

void AdcDataDecoder::fillDynamicData(DynamicData &dynamicData, EncodedAdcData &encodedRetrievedData)
{

    if (encodedRetrievedData.mode == DualChannelMode::ON)
    {
        dynamicData.active_channels.at(CHANNEL_1) = true;
        dynamicData.active_channels.at(CHANNEL_2) = true;
    }
    else
    {
        if (encodedRetrievedData.channelId == CHANNEL_1)
        {
            dynamicData.active_channels.at(CHANNEL_1) = true;
            dynamicData.active_channels.at(CHANNEL_2) = false;
        }
        else
        {
            dynamicData.active_channels.at(CHANNEL_1) = false;
            dynamicData.active_channels.at(CHANNEL_2) = true;
        }

        dynamicData.trigger_source = encodedRetrievedData.channelId;
    }

    dynamicData.frame_duration_ns =
        calculateFrameDuration_ns(encodedRetrievedData.values);
}

double AdcDataDecoder::calculateFrameDuration_ns(
    EncodedAdcValues &encodedRetrievedData)
{
    uint32_t timer_doubleticks_per_frame{
        pullFrameDurationFromEncodedRetrievedData(encodedRetrievedData)};

    constexpr double TIMER_COUNTS_UPWARDS_EDGE_TICKS{2.0};

    return timer_doubleticks_per_frame * DEVICE_TIMER_SINGLE_TICK_DURATION_NS *
           TIMER_COUNTS_UPWARDS_EDGE_TICKS;
}

uint32_t AdcDataDecoder::pullFrameDurationFromEncodedRetrievedData(
    EncodedAdcValues &encodedRetrievedData)
{
    uint32_t timer_doubleticks_per_frame{0};

    for (int i = 3; i >= 0; --i)
    {
        timer_doubleticks_per_frame += (encodedRetrievedData.back() << (i * 8));
        encodedRetrievedData.pop_back();
    }

    return timer_doubleticks_per_frame;
}
