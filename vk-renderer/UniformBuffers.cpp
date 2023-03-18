#include "UniformBuffers.h"

#include "Helpers.h"
#include "Instance.h"
#include "UniformBufferObject.h"

UniformBuffers::UniformBuffers(const std::shared_ptr<VkDevice_T>& device, const int maxFramesInFlight) {
    devicePtr = device;
    framesInFlight = maxFramesInFlight;
        
    createUniformBuffers();
}

UniformBuffers::~UniformBuffers() {
    //ubo cleanup
    for (size_t i = 0; i < framesInFlight; i++) {
        vkDestroyBuffer(devicePtr.get(), uniformBuffers[i], nullptr);
        vkFreeMemory(devicePtr.get(), uniformBuffersMemory[i], nullptr);
    }
}

void UniformBuffers::createUniformBuffers() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffers.resize(framesInFlight);
    uniformBuffersMemory.resize(framesInFlight);
    uniformBuffersMapped.resize(framesInFlight);

    for (size_t i = 0; i < framesInFlight; i++) {
        Instance::getDrawHelper()->createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

        vkMapMemory(devicePtr.get(), uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }
}
