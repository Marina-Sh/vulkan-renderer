#include "Instance.h"
#include "DebugUtils.h"

Instance::Instance() {
    createInstance();
}

Instance::~Instance() {
}

void Instance::createInstance() {
    if (enableValidationLayers) {
        debugMessenger = std::make_unique<VulkanDebugUtils>();
        if (!debugMessenger->checkValidationLayerSupport()) {
            throw std::runtime_error("validation layers requested, but not available!");
        }
    }
        
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
        
    const auto glfwExtensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
    createInfo.ppEnabledExtensionNames = glfwExtensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(debugMessenger->getValidationLayers()->size());
        createInfo.ppEnabledLayerNames = debugMessenger->getValidationLayers()->data();

        debugMessenger->populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    VkInstance newInstance = nullptr;
    if (vkCreateInstance(&createInfo, nullptr, &newInstance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }

    auto instanceDeleter = [](VkInstance_T* instancePtr){vkDestroyInstance(instancePtr, nullptr);};
    instance = std::shared_ptr<VkInstance_T>(newInstance,instanceDeleter);
    
    if (enableValidationLayers) {
        debugMessenger->setupDebugMessenger(instance);
    }
}

std::vector<const char*> Instance::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}
