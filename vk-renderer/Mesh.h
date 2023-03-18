#pragma once
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>


struct Vertex;

class Mesh {
public:
    Mesh() = delete;
    Mesh(const std::shared_ptr<VkDevice_T>& device);
    ~Mesh();

    void setVertexBuffer(const std::vector<Vertex>& vertices);
    void setIndexBuffer(const std::vector<uint32_t>& indices);
    
    VkBuffer getVertexBuffer() const {
        return vertexBuffer;
    }

    VkBuffer getIndexBuffer() const {
        return indexBuffer;
    }

    uint32_t getIndexesSize() const {
        return indexCount;
    }

private:
    VkBuffer vertexBuffer = nullptr;
    VkDeviceMemory vertexBufferMemory = nullptr;
    VkBuffer indexBuffer = nullptr;
    VkDeviceMemory indexBufferMemory = nullptr;
    uint32_t indexCount = 0;

    std::shared_ptr<VkDevice_T> devicePtr = nullptr;
};