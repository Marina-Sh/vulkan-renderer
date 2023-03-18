#pragma once
#include <memory>
#include <string>
#include <vulkan/vulkan_core.h>

class Mesh;

class MeshLoader {
public:
    static std::shared_ptr<Mesh> loadMesh(const std::string& modelPath, const std::shared_ptr<VkDevice_T>& device);
};
