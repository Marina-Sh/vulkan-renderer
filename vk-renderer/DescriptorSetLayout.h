#pragma once
#include <memory>
#include <vulkan/vulkan_core.h>

class DescriptorSetLayout {
public:
    DescriptorSetLayout() = delete;
    DescriptorSetLayout(const std::shared_ptr<VkDevice_T>& device);

    const std::shared_ptr<VkDescriptorSetLayout_T>& getHandle() {
        return descriptorSetLayout;
    }
    
private:
    void createDescriptorSetLayout();
    
    std::shared_ptr<VkDevice_T> devicePtr;
    
    std::shared_ptr<VkDescriptorSetLayout_T> descriptorSetLayout = nullptr; //ubo
};
