#include "CommandBuffer.h"

#include "Instance.h"


CommandBuffer::CommandBuffer(const std::shared_ptr<VkDevice_T>& device, const int maxFramesInFlight) {
    devicePtr = device;

    createCommandPool();
    createCommandBuffers(maxFramesInFlight);
}

CommandBuffer::~CommandBuffer() {
    vkDestroyCommandPool(devicePtr.get(), commandPool, nullptr);
}

void CommandBuffer::createCommandPool() {
    const QueueFamilyIndices indices = Instance::getPhysicalDevice()->getQueueFamilyIndices();
    
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = indices.graphicsFamily.value();

    if (vkCreateCommandPool(devicePtr.get(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

void CommandBuffer::createCommandBuffers(const int maxFramesInFlight) {
    commandBuffers.resize(maxFramesInFlight);
    
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(devicePtr.get(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

