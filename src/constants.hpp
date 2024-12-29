#pragma once

#include <cstdint>

constexpr int X_SIZE{800};
constexpr int Y_SIZE{600};
constexpr int X_POSITION{100};
constexpr int Y_POSITION{100};
constexpr int INPUT_RESOLUTION{4096};
constexpr unsigned int BUFFER_SIZE{X_SIZE};
constexpr uint8_t X_LENGTH{X_SIZE / BUFFER_SIZE};
constexpr uint16_t FREQUENCY_LABEL_TIMEOUT_MS{500};
constexpr uint16_t DEFAULT_TRIGGER_THRESHOLD{(INPUT_RESOLUTION / 2)};
