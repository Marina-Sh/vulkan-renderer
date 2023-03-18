#pragma once
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

class Sync {
public:
    Sync() = delete;
    Sync(const std::shared_ptr<VkDevice_T>& device, const int maxFramesInFlight);
    ~Sync();

    void waitForFrameFence(const uint32_t currentFrame);
    void resetFraneFence(const uint32_t currentFrame);

    VkFence getFrameFence(const uint32_t currentFrame);

    VkSemaphore getFrameAvailableSemaphore(const uint32_t currentFrame);
    VkSemaphore getRenderFinishedSemaphore(const uint32_t currentFrame);
    
private:
    void createSyncObjects();
    
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    std::shared_ptr<VkDevice_T> devicePtr = nullptr;
    int framesInFlight;
};
