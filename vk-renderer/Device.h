#pragma once
#include <vulkan/vulkan_core.h>
#include "PhysicalDevice.h"

#include "DebugUtils.h"

class Device {
    
public:
    Device() = delete;
    Device (const std::shared_ptr<PhysicalDevice>& physicalDevice, ValidationLayers* validationLayers);
    
    void createLogicalDevice(const ValidationLayers* validationLayers, const std::shared_ptr<PhysicalDevice>& physicalDevice);
   
    void waitIdle();

    std::shared_ptr<VkDevice_T> getHandle();

private:
    std::shared_ptr<VkDevice_T> logicalDevice = nullptr;
};
