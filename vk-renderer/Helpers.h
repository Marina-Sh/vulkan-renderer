#pragma once
#include <memory>
#include <stdexcept>
#include <vulkan/vulkan_core.h>


struct createImageInfo {
    uint32_t width;
    uint32_t height;
    uint32_t mipLevels;
    VkSampleCountFlagBits numSamples;
    VkFormat format;
    VkImageTiling tiling;
    VkImageUsageFlags usage;
    VkMemoryPropertyFlags properties;
};

class DrawHelper {
public:
    DrawHelper() = delete;
    DrawHelper(const std::shared_ptr<VkDevice_T>& device);
    ~DrawHelper();
    
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

    void createImage(const createImageInfo& createInfo, VkImage& image, VkDeviceMemory& imageMemory);

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
    
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    
private:
    VkCommandPool tempBuffersCommandPool = nullptr;

    std::shared_ptr<VkDevice_T> devicePtr = nullptr;
    VkQueue graphicsQueue = nullptr;
};
