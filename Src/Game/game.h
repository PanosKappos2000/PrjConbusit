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

        inline static Game* GetMain() {return s_pGame;}

        inline uint8_t RequestShutdown() {
            isRunning = 0;
            return 1;
        }

    private:
        uint8_t isRunning = 0;

        static Game* s_pGame;
    };
}