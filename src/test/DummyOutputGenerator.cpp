#include "DummyOutputGenerator.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <cstdint>
#include <iostream>

uint8_t signal_high{0x0c};
uint8_t signal_low{0x04};

constexpr uint8_t lower_bit{0x00};

void write_single_line(const int deviceFileDescriptor, const uint8_t chId)
{
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 1500; ++j)
        {
            write(deviceFileDescriptor, &lower_bit, 1);
            write(deviceFileDescriptor, &signal_high, 1);
        }

        for (int j = 0; j < 1500; ++j)
        {
            write(deviceFileDescriptor, &lower_bit, 1);
            write(deviceFileDescriptor, &signal_low, 1);
        }
    }

    constexpr uint8_t timer_1st_byte{0x80};
    constexpr uint8_t timer_2nd_byte{0xa2};
    constexpr uint8_t timer_3rd_byte{0x19};
    constexpr uint8_t timer_4th_byte{0x00};

    // timer
    write(deviceFileDescriptor, &timer_1st_byte, 1);
    write(deviceFileDescriptor, &timer_2nd_byte, 1);
    write(deviceFileDescriptor, &timer_3rd_byte, 1);
    write(deviceFileDescriptor, &timer_4th_byte, 1);

    const uint8_t channel_data{0xff - chId};
    write(deviceFileDescriptor, &channel_data, 1);

    constexpr uint8_t both_channels_active{0xfd};
    write(deviceFileDescriptor, &both_channels_active, 1);
}

// TODO: clean-up and parametrize

void DummyOutputGenerator::generate()
{
    int deviceFileDescriptor =
        open("dummy_output", O_CREAT | O_RDWR | O_TRUNC, 0644);

    write_single_line(deviceFileDescriptor, 0);
    write_single_line(deviceFileDescriptor, 1);

    close(deviceFileDescriptor);
}
