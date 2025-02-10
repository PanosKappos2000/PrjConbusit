#pragma once

#include <cstdint>
#include "VendorCode/blitcl.h"

namespace Conbusit
{
    class Game
    {
    public:
        Game();

        void Run();

        ~Game();

    private:
        uint8_t isRunning = 0;
    };
}