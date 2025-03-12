#pragma once

#include <cstdint>
#include <list>

class StatsForTimemarker
{
public:
    void logDuration_us(const uint16_t duration_us);
    uint32_t getAverageDuration_us() const;

private:
    std::list<uint16_t> durations_us;

    const uint16_t number_of_measurements{10};
};
