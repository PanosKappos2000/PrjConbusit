#include "vkRenderer.h"

namespace ConVk
{
    VulkanRenderer* VulkanRenderer::s_pVulkan;

    uint8_t VulkanRenderer::Init(uint32_t window, uint32_t windowHeight)
    {
        s_pVulkan = this;

        if(!CreateInstance(_instance))
            return 0;

        return 1;
    }

    uint8_t CreateInstance(VkInstance& instance)
    {
        uint32_t vkVersion = 0;
        vkEnumerateInstanceVersion(&vkVersion);
        if(vkVersion < VK_VERSION_1_3)
            return 0;

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pNext = nullptr;
        appInfo.pEngineName = "BlitLite";
        appInfo.engineVersion = 1.0;
        appInfo.pApplicationName = "Conbusit";
        appInfo.applicationVersion = 0;
        appInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.flags = 0;
        createInfo.pNext = nullptr;

        createInfo.pApplicationInfo = &appInfo;

        char* enabledLayerNames[1];

        #ifndef NDEBUG
            createInfo.enabledLayerCount = 0;
            enabledLayerNames[0];
            createInfo.ppEnabledLayerNames = enabledLayerNames;
        #endif

        createInfo.enabledExtensionCount = 0;
        createInfo.ppEnabledExtensionNames = nullptr;

        VkResult res = vkCreateInstance(&createInfo, nullptr, &instance);
        if(res != VK_SUCCESS)
            return 0;

        return 1;
    }
}