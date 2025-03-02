#pragma once

#include "StatsForTimemarker.hpp"
#include <chrono>

class Timemarker
{
    using high_resolution_clock = std::chrono::high_resolution_clock;
    using microseconds = std::chrono::microseconds;

public:
    Timemarker(StatsForTimemarker &statistics)
        : statistics(statistics), creation_time(high_resolution_clock::now()) {}

    ~Timemarker()
    {
        statistics.logDuration_us(duration_cast<microseconds>(
                                      high_resolution_clock::now() - creation_time)
                                      .count());
    }

private:
    StatsForTimemarker &statistics;
    const std::chrono::high_resolution_clock::time_point creation_time;
};
