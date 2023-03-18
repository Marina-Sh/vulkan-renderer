#pragma once
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "DebugUtils.h"
#include "Helpers.h"
#include "PhysicalDevice.h"


class Instance {
public:
    Instance();
    ~Instance();
    
    void createInstance();
    const std::shared_ptr<VkInstance_T>& getHandle() {
        return instance;
    }

    const ValidationLayers* getValidationLayers() const {
        if(debugMessenger != nullptr) {
            return debugMessenger->getValidationLayers();
        }
        return nullptr;
    }

    //service locator functions
    static const std::shared_ptr<PhysicalDevice>& getPhysicalDevice() {
        if (physicalDevice == nullptr) {
            throw std::runtime_error("physical device isn't set!");
        }
        return physicalDevice;
    }

    static void providePhysicalDevice(const std::shared_ptr<PhysicalDevice>& buffersManager) {
        physicalDevice = buffersManager;
    }

    static std::shared_ptr<DrawHelper> getDrawHelper() {
        if (drawHelper.expired()) {
            throw std::runtime_error("draw buffer isn't set!");
        }
        return drawHelper.lock();
    }

    static void provideDrawHelper(const std::shared_ptr<DrawHelper>& buffersManager) {
        drawHelper = buffersManager;
    }
    
    
private:
    std::vector<const char*> getRequiredExtensions();

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
    
    std::shared_ptr<VkInstance_T> instance = nullptr;
    std::unique_ptr<VulkanDebugUtils> debugMessenger = nullptr;

    inline static std::shared_ptr<PhysicalDevice> physicalDevice = nullptr;
    inline static std::weak_ptr<DrawHelper> drawHelper;
};
