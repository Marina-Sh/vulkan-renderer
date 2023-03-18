#pragma once

#include <functional>
#include <memory>
#include <vulkan/vulkan_core.h>

class Surface {
public:
    Surface() = delete;
    Surface(const std::function<VkSurfaceKHR(VkInstance)>& creationFunction,  const std::shared_ptr<VkInstance_T>& instance);

    const std::shared_ptr<VkSurfaceKHR_T>& getHandle() const {
        return surface;
    }

private:
    std::shared_ptr<VkSurfaceKHR_T> surface = nullptr;
};
