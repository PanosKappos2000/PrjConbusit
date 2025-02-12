#pragma once

#include <cstdint>
#include "ConVulkan/vkRenderer.h"
#include "VendorCode/blitcl.h"

namespace Conbusit
{
    enum class ActiveRenderer : uint8_t
    {
        Vulkan = 0,
        Max = 1
    };

    class RenderingSystem
    {
    public:
        RenderingSystem();

        ~RenderingSystem();

        inline static RenderingSystem* GetSystem() { return s_pSystem; } 

        ConVk::VulkanRenderer* pVK = nullptr;

    private:

        ConVk::VulkanRenderer _vk;

        static RenderingSystem* s_pSystem;
    };

    void CreateVulkanRenderer(ConVk::VulkanRenderer& vk, uint32_t windowWidth, uint32_t windowHeight);
}