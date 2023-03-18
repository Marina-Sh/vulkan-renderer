#pragma once
#include <array>
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "Window.h"

class PhysicalDevice;
class Device;
struct SwapChainSupportDetails;
struct QueueFamilyIndices;

class SwapChain {
public:
    SwapChain() = delete;
    SwapChain(const std::shared_ptr<VkDevice_T>& device,
        const std::shared_ptr<VkSurfaceKHR_T>& surface,
        const std::shared_ptr<Window>& window
        );
    ~SwapChain();

    std::shared_ptr<VkRenderPass_T> getRenderPass() {
        return renderPass;
    }

    VkRenderPassBeginInfo getRenderPassInfo(const uint32_t imageIndex);
    
    VkSwapchainKHR getHandle() const {
        return swapChain;
    }
    
    void recreateSwapChain();

    float getAspectRatio() {
        return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
    }

private:
    void createSwapChain();

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void createImageViews();
    void createFrameBuffers();
    void createRenderPass();

    void cleanupSwapChain();

    void createDepthResources();
    bool hasStencilComponent(VkFormat format);
    VkFormat findDepthFormat();

    void createColorResources();

    std::array<VkClearValue, 2> clearValues {};
    
    VkSwapchainKHR swapChain;

    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    
    std::vector<VkImageView> imageViews;
    std::vector<VkFramebuffer> framebuffers;
    std::shared_ptr<VkRenderPass_T> renderPass;

    std::shared_ptr<VkDevice_T> devicePtr = nullptr;
    std::shared_ptr<VkSurfaceKHR_T> surfacePtr = nullptr;
    std::shared_ptr<Window> windowPtr = nullptr;
    
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkImage colorImage;
    VkDeviceMemory colorImageMemory;
    VkImageView colorImageView;
};
