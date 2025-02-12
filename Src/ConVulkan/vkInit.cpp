#include "vkRenderer.h"

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT createInfo, 
VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
    instance, "vkCreateDebugUtilsMessengerEXT");
    if(func != nullptr)
    {
        return func(instance, &createInfo, nullptr, pDebugMessenger);
    }
}

namespace ConVk
{
    VulkanRenderer* VulkanRenderer::s_pVulkan;

    uint8_t VulkanRenderer::Init(uint32_t window, uint32_t windowHeight)
    {
        s_pVulkan = this;

        if(!CreateInstance(_instance, _debugMessenger))
            return 0;

        return 1;
    }

    uint8_t CreateInstance(VkInstance& instance, VkDebugUtilsMessengerEXT& debugMessenger)
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
            uint32_t layerPropertyCount = 0;
            vkEnumerateInstanceLayerProperties(&layerPropertyCount, nullptr);
            Blitcl::DynamicArray<VkLayerProperties> props(layerPropertyCount);
            vkEnumerateInstanceLayerProperties(&layerPropertyCount, props.Data());

            uint8_t found = 0;
            for(size_t i = 0; i < props.GetSize(); ++i)
            {
                if(!strcmp(props[i].layerName, "VK_LAYER_KHRONOS_validation"))
                {
                    found = 1;
                    break;
                }
            }

            if(found)
            {
                createInfo.enabledLayerCount = 1;
                enabledLayerNames[0] = "VK_LAYER_KHRONOS_validation";
                createInfo.ppEnabledLayerNames = enabledLayerNames;
            }
            else
            {
                createInfo.enabledLayerCount = 0;
            }


        #endif

        createInfo.enabledExtensionCount = 0;
        createInfo.ppEnabledExtensionNames = nullptr;

        VkResult res = vkCreateInstance(&createInfo, nullptr, &instance);
        if(res != VK_SUCCESS)
            return 0;

        return 1;
    }

    uint8_t CreateDebugMessenger(VkDebugUtilsMessengerEXT& debugMessenger, VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;

        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        //createInfo.pfnUserCallback = debugCallback;
    }
}