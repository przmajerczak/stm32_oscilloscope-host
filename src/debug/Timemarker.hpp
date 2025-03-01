#pragma once

#include <chrono>
#include <iostream>

class Timemarker
{
    using high_resolution_clock = std::chrono::high_resolution_clock;
    using microseconds = std::chrono::microseconds;

public:
    Timemarker(const char *label)
        : label(label), creation_time(high_resolution_clock::now()) {}

    ~Timemarker()
    {
        std::cout << "Timemarker " << label << " duration: "
                  << duration_cast<microseconds>(high_resolution_clock::now() -
                                                 creation_time)
                         .count()
                  << " microseconds." << std::endl;
    }

private:
    const char *label;
    const std::chrono::high_resolution_clock::time_point creation_time;
};
