#include "renderer.h"
#include "Game/game.h"

namespace Conbusit
{
    RenderingSystem* RenderingSystem::s_pSystem = nullptr;

    RenderingSystem::RenderingSystem()
    {
        s_pSystem = this;

        CreateVulkanRenderer(_vk, CON_WINDOW_WIDTH, CON_WINDOW_HEIGHT);
    }

    RenderingSystem::~RenderingSystem()
    {
        s_pSystem = nullptr;
    }

    void CreateVulkanRenderer(ConVk::VulkanRenderer& vk, uint32_t windowWidth, uint32_t windowHeight)
    {
        if(vk.Init(windowWidth, windowHeight))
        {
            RenderingSystem::GetSystem()->pVK = &vk;
        }
    }

    void ShutdownRenderers()
    {

    }
}