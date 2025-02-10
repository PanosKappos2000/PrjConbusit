#include "control.h"
#include "game.h"

namespace Conbusit
{
    uint8_t ActivateGameControls()
    {
        if(!Core::RegisterEvent(Core::BlitEventType::EngineShutdown, nullptr, WindowShutdownEvent))
            return 0;

        if(!Core::RegisterEvent(Core::BlitEventType::KeyPressed, nullptr, KeyEvent))
            return 0;
        
        return 1;
    }

    uint8_t WindowShutdownEvent(Core::BlitEventType type, void* pSender, void* pListener, Core::EventContext data)
    {
        Game* pGame = Game::GetMain();
        return pGame->RequestShutdown();
    }

    uint8_t KeyEvent(Core::BlitEventType type, void* pSender, void* pListener, Core::EventContext data)
    {
        Core::BlitKey key = static_cast<Core::BlitKey>(data.data.ui16[0]);

        if(type == Core::BlitEventType::KeyPressed)
        {
            switch(key)
            {
                case Core::BlitKey::__ESCAPE:
                {
                    Game::GetMain()->RequestShutdown();
                    break;
                }
                default:
                {
                    break;
                }
            }
        }

        return 0;
    }
}