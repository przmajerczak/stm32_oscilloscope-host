#include "StatsForTimemarker.hpp"

#include <numeric>

void StatsForTimemarker::logDuration_us(const uint16_t duration_us)
{
    durations_us.push_back(duration_us);

    while (durations_us.size() > number_of_measurements)
    {
        durations_us.pop_front();
    }
}

uint32_t StatsForTimemarker::getAverageDuration_us() const
{
    if (durations_us.empty())
    {
        return 0;
    }

    return std::accumulate(durations_us.begin(), durations_us.end(), 0) /
           durations_us.size();
}
