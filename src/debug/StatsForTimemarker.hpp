#pragma once

#include <cstdint>
#include <limits>
#include <list>
#include <numeric>

class StatsForTimemarker
{
public:
    void logDuration_us(const uint16_t duration_us)
    {
        durations_us.push_back(duration_us);

        while (durations_us.size() > number_of_measurements)
        {
            durations_us.pop_front();
        }
    }

    uint32_t getAverageDuration_us() const
    {

        if (durations_us.empty())
        {
            return std::numeric_limits<double>::infinity();
        }

        return std::accumulate(durations_us.begin(), durations_us.end(), 0) /
               durations_us.size();
    }

private:
    std::list<uint16_t> durations_us;

    const uint16_t number_of_measurements{100};
};
