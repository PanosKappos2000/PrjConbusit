#include "game.h"
#include "control.h"
#include "Window/platform.h"
#include "Window/events.h"
#include "Rendering/renderer.h"

namespace Conbusit
{
    Game* Game::s_pGame = nullptr;

    Game::Game()
    {
        isRunning = 1;
        s_pGame = this;
    }

    void Game::Run()
    {
        Blitcl::SmartPointer<Core::EventSystemContext, Blitcl::AllocationType::Engine> eventSystem;
        Blitcl::SmartPointer<Core::InputSystemContext, Blitcl::AllocationType::Engine> inputSystem;

        BLIT_ASSERT(ActivateGameControls());

        BLIT_ASSERT(Platform::WindowSystemInit("Conbusit", 100, 100, CON_WINDOW_WIDTH, CON_WINDOW_HEIGHT))

        Blitcl::SmartPointer<RenderingSystem, Blitcl::AllocationType::Renderer> renderer;

        while(isRunning)
        {
            if(!Platform::PumpMessages())
            {
                isRunning = 0;
            }

            Core::UpdateInput(0);
        }
    }

    Game::~Game()
    {

    }


}

Blitcl::MemoryManagerState* Blitcl::MemoryManagerState::s_pState;

int main()
{
    Blitcl::MemoryManagerState memory;

    {
        Conbusit::Game game;

        game.Run();
    }
}