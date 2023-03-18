#include "Mesh.h"

#include "Device.h"
#include "Helpers.h"
#include "Instance.h"
#include "Vertex.h"

Mesh::Mesh(const std::shared_ptr<VkDevice_T>& device) {
    devicePtr = device;
}

Mesh::~Mesh() {
    if (indexBuffer != nullptr) {
        vkDestroyBuffer(devicePtr.get(), indexBuffer, nullptr);
        vkFreeMemory(devicePtr.get(), indexBufferMemory, nullptr);
        indexBuffer = nullptr;
    }
    
    if (vertexBuffer != nullptr) {
        vkDestroyBuffer(devicePtr.get(), vertexBuffer, nullptr);
        vkFreeMemory(devicePtr.get(), vertexBufferMemory, nullptr);
        vertexBuffer = nullptr;
    }
}

void Mesh::setVertexBuffer(const std::vector<Vertex>& vertices)  {
    if (vertexBuffer != nullptr) {
        vkDestroyBuffer(devicePtr.get(), vertexBuffer, nullptr);
        vkFreeMemory(devicePtr.get(), vertexBufferMemory, nullptr);
        vertexBuffer = nullptr;
    }
    
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    const auto drawHelper = Instance::getDrawHelper();
    drawHelper->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(devicePtr.get(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(devicePtr.get(), stagingBufferMemory);

    drawHelper->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

    drawHelper->copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(devicePtr.get(), stagingBuffer, nullptr);
    vkFreeMemory(devicePtr.get(), stagingBufferMemory, nullptr);
}

void Mesh::setIndexBuffer(const std::vector<uint32_t>& indices) {
    if (indexBuffer != nullptr) {
        vkDestroyBuffer(devicePtr.get(), indexBuffer, nullptr);
        vkFreeMemory(devicePtr.get(), indexBufferMemory, nullptr);
        indexBuffer = nullptr;
    }
    
    indexCount = static_cast<uint32_t>(indices.size());
    
    VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

    const auto drawHelper = Instance::getDrawHelper();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    drawHelper->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(devicePtr.get(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t) bufferSize);
    vkUnmapMemory(devicePtr.get(), stagingBufferMemory);

    drawHelper->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    drawHelper->copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(devicePtr.get(), stagingBuffer, nullptr);
    vkFreeMemory(devicePtr.get(), stagingBufferMemory, nullptr);
}
