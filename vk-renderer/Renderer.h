#pragma once
#include <memory>
#include <vulkan/vulkan_core.h>

#include "CommandBuffer.h"
#include "DescriptorSetLayout.h"
#include "DescriptorSets.h"
#include "GraphicsPipeline.h"
#include "Mesh.h"
#include "SwapChain.h"
#include "Sync.h"
#include "Texture.h"
#include "UniformBuffers.h"


class Window;

class Renderer {
public:
    Renderer() = delete;
    Renderer(const std::shared_ptr<VkDevice_T>& device,
        const std::shared_ptr<Window>& window,
        const std::shared_ptr<VkSurfaceKHR_T>&surface,
        const std::shared_ptr<Mesh>& mesh,
        const std::shared_ptr<Texture>& texture
        );

    void drawFrame();

    void setFramebufferResized() {
        framebufferResized = true;
    }

    void updateUniformBuffer(float aspectRatio);
    void recordCommandBuffer(VkCommandBuffer currentBuffer, uint32_t imageIndex);
    
private:

    std::shared_ptr<VkDevice_T> devicePtr;

    std::shared_ptr<Mesh> meshPtr = nullptr;

    float currentRotation = 0.f;
    float rotationSpeed = 10.f;

    std::unique_ptr<Sync> syncObjects = nullptr;
    const int maxFramesInFlight = 2;

    uint32_t currentFrame = 0;
    
    bool framebufferResized = false;

    std::unique_ptr<SwapChain> swapChain = nullptr;
    std::unique_ptr<DescriptorSetLayout> descriptorSetLayout = nullptr;
    std::unique_ptr<GraphicsPipeline> graphicsPipeline = nullptr;
    std::unique_ptr<CommandBuffer> commandBuffer = nullptr;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    std::shared_ptr<UniformBuffers> uniformBuffers = nullptr;
    std::shared_ptr<DescriptorSets> descriptorSets = nullptr;
};

