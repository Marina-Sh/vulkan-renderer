#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>

#include "Device.h"

class CommandBuffer {
public:
    CommandBuffer() = delete;
    CommandBuffer(const std::shared_ptr<VkDevice_T>& device, const int maxFramesInFlight);
    ~CommandBuffer();

    VkCommandBuffer getFrameCommandBuffer(const uint32_t currentFrame) const {
        return commandBuffers[currentFrame];
    }

private:
    void createCommandPool();
    void createCommandBuffers(const int maxFramesInFlight);
    
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    std::shared_ptr<VkDevice_T> devicePtr = nullptr;
};
