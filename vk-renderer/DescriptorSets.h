#pragma once
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

class DescriptorSets {
public:
    DescriptorSets() = delete;
    DescriptorSets(const std::shared_ptr<VkDevice_T>& device,
        const std::shared_ptr<VkDescriptorSetLayout_T>& descriptorSetLayout,
        const std::vector<VkBuffer>& uniformBuffers,
        VkImageView textureImageView,
        VkSampler textureSampler,
        const int maxFramesInFlight);
    
    ~DescriptorSets();

    VkDescriptorSet getFrameDescriptorSet(const uint32_t frameIndex) const {
        return descriptorSets[frameIndex];
    }

private:
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    void createDescriptorPool();
    void createDescriptorSets(const std::vector<VkBuffer>& uniformBuffers, VkImageView textureImageView, VkSampler textureSampler);
    
    std::shared_ptr<VkDevice_T> devicePtr = nullptr;
    std::shared_ptr<VkDescriptorSetLayout_T> descriptorSetLayoutPtr = nullptr; 
    int framesInFlight;
    
};
