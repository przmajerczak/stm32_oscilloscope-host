#pragma once

#include <cstdint>

constexpr uint16_t X_DISPLAY_RESOLUTION{800};
constexpr uint16_t Y_DISPLAY_RESOLUTION{600};
constexpr uint16_t DISPLAY_MARGIN_WIDTH{60};
constexpr uint16_t X_WINDOW_SIZE{X_DISPLAY_RESOLUTION +
                                 (2 * DISPLAY_MARGIN_WIDTH)};
constexpr uint16_t Y_WINDOW_SIZE{Y_DISPLAY_RESOLUTION +
                                 (2 * DISPLAY_MARGIN_WIDTH)};
constexpr uint16_t X_INITIAL_WINDOW_POSITION{100};
constexpr uint16_t Y_INITIAL_WINDOW_POSITION{100};
constexpr uint16_t INPUT_SIGNAL_MAX{4096};
constexpr uint16_t INPUT_SIGNAL_MIN{0};
constexpr uint16_t INPUT_SIGNAL_RESOLUTION{INPUT_SIGNAL_MAX - INPUT_SIGNAL_MIN};
constexpr uint32_t DEFAULT_HORIZONTAL_RESOLUTION_NS{10000000};
constexpr double DEVICE_TIME_MEASUREMENT_TIMER_FREQ_MHZ{168.0};
constexpr double NANOSECONDS_PER_MHZ{1000.0};
constexpr double DEVICE_TIMER_SINGLE_TICK_DURATION_NS{
    NANOSECONDS_PER_MHZ / DEVICE_TIME_MEASUREMENT_TIMER_FREQ_MHZ};
constexpr uint16_t LABELS_REFRESH_RATE_MS{500};
constexpr uint16_t DEFAULT_TRIGGER_THRESHOLD{Y_DISPLAY_RESOLUTION / 2};
constexpr uint16_t INVALID_VALUE{0xffff};
constexpr uint8_t NUMBER_OF_CHANNELS{2};
constexpr std::size_t CHANNEL_1{0};
constexpr std::size_t CHANNEL_2{1};
constexpr float VERY_BOLD{4.0f};
constexpr float BOLD{1.5f};
constexpr float NEUTRAL{1.0f};
constexpr float MAX_VOLTAGE_mV{3300.0f};
constexpr float MIN_VOLTAGE_mV{0.0f};
constexpr float ABSOULTE_VERTICAL_RESOLUTION_mV{MAX_VOLTAGE_mV -
                                                MIN_VOLTAGE_mV};
constexpr float FONT_SIZE_LARGE{24.0f};
constexpr float FONT_SIZE_SMALL{20.0f};
constexpr float COLOR_RGB_WHITE{1.0};
constexpr float COLOR_RGB_LIGHT_GRAY{0.6};
constexpr float COLOR_RGB_DARK_GRAY{0.3};
constexpr const char *FONT_PATH{"resources/UbuntuMono-R.ttf"};
