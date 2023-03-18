#pragma once
#include <memory>
#include <vulkan/vulkan_core.h>

class GraphicsPipeline {
public:
    GraphicsPipeline() = delete;
    GraphicsPipeline(const std::shared_ptr<VkDevice_T>& device, const std::shared_ptr<VkRenderPass_T>& renderPass, const std::shared_ptr<VkDescriptorSetLayout_T>& descriptorSetLayout); 
    ~GraphicsPipeline();

    VkPipeline getGraphicsPipeline() const {
        return graphicsPipeline;
    }
    
    VkPipelineLayout getPipelineLayout() const {
        return pipelineLayout;
    }

private:
    void createGraphicsPipeline();
    
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;

    std::shared_ptr<VkDevice_T> devicePtr = nullptr;
    std::shared_ptr<VkRenderPass_T> renderPassPtr = nullptr;
    std::shared_ptr<VkDescriptorSetLayout_T> descriptorSetLayoutPtr = nullptr; //ubo
};
