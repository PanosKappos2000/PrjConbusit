#pragma once

#include <cstdint>
#include <blitcl.h>

#define CON_WINDOW_WIDTH    1280
#define CON_WINDOW_HEIGHT   768

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