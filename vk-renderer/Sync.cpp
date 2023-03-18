#include "Sync.h"

#include <stdexcept>

Sync::Sync(const std::shared_ptr<VkDevice_T>& device, const int maxFramesInFlight) {
    devicePtr = device;
    framesInFlight = maxFramesInFlight;
    createSyncObjects();
}

Sync::~Sync() {
    for (int i = 0; i < framesInFlight; i++) {
        vkDestroySemaphore(devicePtr.get(), renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(devicePtr.get(), imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(devicePtr.get(), inFlightFences[i], nullptr);
    }
}

void Sync::waitForFrameFence(const uint32_t currentFrame) {
    vkWaitForFences(devicePtr.get(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
}

void Sync::resetFraneFence(const uint32_t currentFrame) {
    vkResetFences(devicePtr.get(), 1, &inFlightFences[currentFrame]);
}

VkFence Sync::getFrameFence(const uint32_t currentFrame) {
    return inFlightFences[currentFrame];
}

VkSemaphore Sync::getFrameAvailableSemaphore(const uint32_t currentFrame) {
    return imageAvailableSemaphores[currentFrame];
}

VkSemaphore Sync::getRenderFinishedSemaphore(const uint32_t currentFrame) {
    return renderFinishedSemaphores[currentFrame];
}

void Sync::createSyncObjects() {
    imageAvailableSemaphores.resize(framesInFlight);
    renderFinishedSemaphores.resize(framesInFlight);
    inFlightFences.resize(framesInFlight);
    
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (int i = 0; i < framesInFlight; i++) {
        if (vkCreateSemaphore(devicePtr.get(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(devicePtr.get(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(devicePtr.get(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
    }
}
