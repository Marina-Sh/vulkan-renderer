#pragma once
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

class UniformBuffers {
public:
    UniformBuffers() = delete;
    UniformBuffers(const std::shared_ptr<VkDevice_T>& device, const int maxFramesInFlight);
    ~UniformBuffers();

    void* getFrameUniformBuffer(const uint32_t frameIndex) {
        return uniformBuffersMapped[frameIndex];
    }

    const std::vector<VkBuffer>& getBuffers() {
        return uniformBuffers;    
    }
    
private:
    void createUniformBuffers();
    
    std::shared_ptr<VkDevice_T> devicePtr = nullptr;
    int framesInFlight;
    
    //ubo
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;
};
