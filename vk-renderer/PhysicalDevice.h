#pragma once
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};


class PhysicalDevice {
public:
    PhysicalDevice() = delete;
    PhysicalDevice(const std::shared_ptr<VkInstance_T>& instance, const std::shared_ptr<VkSurfaceKHR_T>& surface);

    QueueFamilyIndices getQueueFamilyIndices();
    SwapChainSupportDetails getSwapChainSupportDetails();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, const std::function<bool(const VkFormatProperties&)>& isFormatSupported);

    VkPhysicalDevice getHandle() const {
        return physicalDevice;
    }

    const std::vector<const char*>& getDeviceExtensions() const {
        return deviceExtensions;
    }

    float getMaxAnisotropy() const {
        return maxAnisotropy;
    }

    VkSampleCountFlagBits getMsaaSamples() const {
        return msaaSamples;
    }
    
private:
    void pickPhysicalDevice(VkInstance instance);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    int rateDeviceSuitability(VkPhysicalDevice device);
    VkSampleCountFlagBits getMaxUsableSampleCount();

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    float maxAnisotropy = 0.f;
    
    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    
    VkPhysicalDevice physicalDevice = nullptr;

    std::shared_ptr<VkSurfaceKHR_T> surfacePtr = nullptr;
    
};