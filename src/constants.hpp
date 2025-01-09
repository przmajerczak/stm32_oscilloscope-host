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
constexpr uint16_t DISPLAY_BUFFER_SIZE{X_DISPLAY_RESOLUTION};
constexpr uint8_t X_LENGTH{X_DISPLAY_RESOLUTION / DISPLAY_BUFFER_SIZE};
constexpr uint16_t FREQUENCY_LABEL_TIMEOUT_MS{500};
constexpr uint16_t DEFAULT_TRIGGER_THRESHOLD{Y_DISPLAY_RESOLUTION / 2};
constexpr uint16_t INVALID_VALUE{0xffff};
constexpr float VERY_BOLD{4.0f};
constexpr float BOLD{1.5f};
constexpr float NEUTRAL{1.0f};
constexpr float MAX_VOLTAGE_mV{3300.0f};
constexpr float MIN_VOLTAGE_mV{0.0f};
constexpr float ABSOULTE_VERTICAL_RESOLUTION_mV{MAX_VOLTAGE_mV -
                                                MIN_VOLTAGE_mV};
constexpr float FONT_SIZE_LARGE{0.16f};
constexpr float FONT_SIZE_SMALL{0.12f};
constexpr float COLOR_RGB_WHITE{1.0};
constexpr float COLOR_RGB_LIGHT_GRAY{0.6};
constexpr float COLOR_RGB_DARK_GRAY{0.3};
