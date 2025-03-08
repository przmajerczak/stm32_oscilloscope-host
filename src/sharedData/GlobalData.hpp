#pragma once

#include "sharedData/TimemarkersData.hpp"
#include "sharedData/VerticalBoundsData.hpp"
#include "sharedData/constants.hpp"
#include <cstdint>

class GlobalData
{
public:
    VerticalBoundsData verticalBoundsData;
    double frame_duration_ns;
    uint32_t horizontal_resolution_ns{DEFAULT_HORIZONTAL_RESOLUTION_NS};
    TimemarkersData timemarkersData;
};
