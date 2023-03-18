#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>

using ValidationLayers = const std::vector<const char*>; 

class VulkanDebugUtils {
public:
    VulkanDebugUtils() = default;
    ~VulkanDebugUtils();
    
    void setupDebugMessenger(std::shared_ptr<VkInstance_T> instance);
    
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    bool checkValidationLayerSupport();

    const ValidationLayers* getValidationLayers() const {
        return &validationLayers;
    }
    
private:
    static VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

    static void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

    void destroy();
    
    ValidationLayers validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    
    VkDebugUtilsMessengerEXT debugMessenger = nullptr;
    std::shared_ptr<VkInstance_T> instancePtr = nullptr;
    
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
    {
        //TODO: can make message severity a parameter
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        }
        return VK_FALSE;
    }
};