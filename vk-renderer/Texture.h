#pragma once

#include <memory>

#include <string>

#include <vulkan/vulkan_core.h>


class Texture {
public:
    Texture() = delete;
    Texture(const std::shared_ptr<VkDevice_T>& device, const std::string& texturePath);
    ~Texture();
    void createTextureImage(const std::string& texturePath);

    VkImageView getImageView() const {
        return imageView;
    }

    VkSampler getTextureSampler() const {
        return sampler;
    }

private:
    void createTextureImageView();
    void createTextureSampler();

    void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

    VkImage image;
    VkDeviceMemory imageMemory;

    VkImageView imageView;
    VkSampler sampler;

    uint32_t mipLevels;

    std::shared_ptr<VkDevice_T> devicePtr = nullptr;
};
