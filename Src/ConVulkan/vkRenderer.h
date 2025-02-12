#pragma once

#include <vulkan/vulkan.h>

namespace ConVk
{
    class VulkanRenderer
    {
    public:
        uint8_t Init(uint32_t windowWidth, uint32_t windowHeight);

        inline static VulkanRenderer* GetVulkan() { return s_pVulkan; }

    public:
        VkDevice _device;

    private:

        static VulkanRenderer* s_pVulkan;

        VkInstance _instance;
    };

    uint8_t CreateInstance(VkInstance& instance);
}