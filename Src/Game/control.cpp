#include "control.h"
#include "game.h"

namespace Conbusit
{
    uint8_t ActivateGameControls()
    {
        if(!Core::RegisterEvent(Core::BlitEventType::EngineShutdown, nullptr, WindowShutdownEvent))
            return 0;
        
        return 1;
    }

    uint8_t WindowShutdownEvent(Core::BlitEventType type, void* pSender, void* pListener, Core::EventContext data)
    {
        Game* pGame = Game::GetMain();
        return pGame->RequestShutdown();
    }
}