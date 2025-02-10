#pragma once

#include <cstdint>

namespace Platform
{
    // Sets up a window to host the output of the application, implemented differently depending on the platform
    uint8_t WindowSystemInit(const char* appName, int32_t x, int32_t y, uint32_t width, uint32_t height);

    void PlatformShutdown();

    uint8_t PumpMessages();
}