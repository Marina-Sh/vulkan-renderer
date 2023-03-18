#include "Device.h"

#include <set>
#include <stdexcept>
#include <vector>

Device::Device(const std::shared_ptr<PhysicalDevice>& physicalDevice,
    ValidationLayers* validationLayers) {
    createLogicalDevice(validationLayers, physicalDevice);
}

void Device::createLogicalDevice(const ValidationLayers* validationLayers, const std::shared_ptr<PhysicalDevice>& physicalDevice) {
    QueueFamilyIndices queueFamilyIndices = physicalDevice->getQueueFamilyIndices();
    
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value()};

    const float queuePriority = 1.0f;
    for (const uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    vkGetPhysicalDeviceFeatures(physicalDevice->getHandle(), &deviceFeatures);
    deviceFeatures.samplerAnisotropy = VK_TRUE;
    deviceFeatures.sampleRateShading = VK_TRUE; // enable sample shading feature for the device

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;
    auto deviceExtensions = physicalDevice->getDeviceExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (validationLayers != nullptr) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers->size());
        createInfo.ppEnabledLayerNames = validationLayers->data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    VkDevice device;
    if (vkCreateDevice(physicalDevice->getHandle(), &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }
    
    auto deviceDeleter = [](VkDevice_T * device){vkDestroyDevice(device, nullptr);};
    logicalDevice = std::shared_ptr<VkDevice_T>(device, deviceDeleter);
}


void Device::waitIdle() {
    vkDeviceWaitIdle(logicalDevice.get());
}

std::shared_ptr<VkDevice_T> Device::getHandle() {
    return logicalDevice;
}
