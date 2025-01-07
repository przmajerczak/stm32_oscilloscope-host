#pragma once

#include <cstdint>

constexpr uint16_t X_DISPLAY_RESOLUTION{800};
constexpr uint16_t Y_DISPLAY_RESOLUTION{600};
constexpr uint16_t DISPLAY_MARGIN_WIDTH{60};
constexpr uint16_t X_WINDOW_SIZE{X_DISPLAY_RESOLUTION + (2 * DISPLAY_MARGIN_WIDTH)};
constexpr uint16_t Y_WINDOW_SIZE{Y_DISPLAY_RESOLUTION + (2 * DISPLAY_MARGIN_WIDTH)};
constexpr uint16_t X_INITIAL_WINDOW_POSITION{100};
constexpr uint16_t Y_INITIAL_WINDOW_POSITION{100};
constexpr uint16_t INPUT_SIGNAL_RESOLUTION{4096};
constexpr uint16_t DISPLAY_BUFFER_SIZE{X_DISPLAY_RESOLUTION};
constexpr uint8_t X_LENGTH{X_DISPLAY_RESOLUTION / DISPLAY_BUFFER_SIZE};
constexpr uint16_t FREQUENCY_LABEL_TIMEOUT_MS{500};
constexpr uint16_t DEFAULT_TRIGGER_THRESHOLD{(INPUT_SIGNAL_RESOLUTION / 2)};
constexpr uint16_t INVALID_VALUE{0xffff};
constexpr float BOLD_THICKNESS{4.0f};
constexpr float MAX_VOLTAGE_mV{3300.0f};
constexpr float MIN_VOLTAGE_mV{0.0f};
constexpr float FONT_SIZE_LARGE{0.16f};
constexpr float FONT_SIZE_SMALL{0.08f};
constexpr float COLOR_RGB_WHITE{1.0};
constexpr float COLOR_RGB_LIGHT_GRAY{0.6};
constexpr float COLOR_RGB_DARK_GRAY{0.3};
