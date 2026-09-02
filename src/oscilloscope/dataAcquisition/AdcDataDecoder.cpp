#include "AdcDataDecoder.hpp"

#include "sharedData/constants.hpp"
#include "sharedData/DynamicData.hpp"

MillivoltsArray AdcDataDecoder::decodeAdcValuesInto_mV(const EncodedAdcValues &encoded_values)
{
    MillivoltsArray decoded_values;

    auto current_encoded_values{encoded_values.begin()};
    auto next_encoded_values{std::next(current_encoded_values, 1)};

    for (std::size_t i = 0; i < SAMPLES_PER_TRANSMISSION; ++i)
    {
        AdcValue decoded_value = *current_encoded_values + (*next_encoded_values << 8);
        decoded_values.at(i) = scaleAdcTo_mV(decoded_value);

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

float AdcDataDecoder::scaleAdcTo_mV(const AdcValue adc_value) const
{
    constexpr float RESOLUTION_FACTOR{((ABSOULTE_VERTICAL_RESOLUTION_mV) / (static_cast<float>(INPUT_SIGNAL_RESOLUTION)))};
    constexpr float SIGNAL_FLOOR{RESOLUTION_FACTOR * static_cast<float>(INPUT_SIGNAL_MIN) + MIN_VOLTAGE_mV};

    return static_cast<float>(adc_value) * RESOLUTION_FACTOR + SIGNAL_FLOOR;
}
