#pragma once

#include "debug/StatsForTimemarker.hpp"

struct TimemarkersData
{
    StatsForTimemarker totalDataAnalyzeDuration;
    StatsForTimemarker totalFrameDisplayDuration;
    StatsForTimemarker customNonLibraryFrameDisplayDuration;
    StatsForTimemarker totalDataRetrievalAndDecodingDuration;
    StatsForTimemarker singleFrameDataRetrievalDuration;
};
