#pragma once

#include <vulkan/vulkan.h>
#include <blitcl.h>

namespace ConVk
{
    class VulkanRenderer
    {
    public:
        uint8_t Init(uint32_t windowWidth, uint32_t windowHeight);

        inline static VulkanRenderer* GetVulkan() { return s_pVulkan; }

        void Shutdown();

    public:
        VkDevice _device;

    private:

        static VulkanRenderer* s_pVulkan;

        VkInstance _instance;

        VkDebugUtilsMessengerEXT _debugMessenger;
    };

    uint8_t CreateInstance(VkInstance& instance, VkDebugUtilsMessengerEXT& debugMessenger);

    uint8_t CreateDebugMessenger(VkDebugUtilsMessengerEXT& debugMessenger, VkDebugUtilsMessengerCreateInfoEXT& createInfo);
}