#include "Surface.h"

#include <stdexcept>

Surface::Surface(const std::function<VkSurfaceKHR(VkInstance)>& creationFunction,  const std::shared_ptr<VkInstance_T>& instance) {

    if (!creationFunction){
        throw std::runtime_error("failed to create window surface!");
    }
    
    auto surfaceDeleter = [instancePtr = instance](VkSurfaceKHR_T* surfacePtr) {
        vkDestroySurfaceKHR(instancePtr.get(), surfacePtr, nullptr);
    };
    surface = std::shared_ptr<VkSurfaceKHR_T>(creationFunction(instance.get()),surfaceDeleter);
}
