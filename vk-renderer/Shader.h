#pragma once
#include <map>
#include <memory>
#include <vulkan/vulkan_core.h>
#include <string>
#include <vector>

class Shader {
public:
    enum class Type {
        Vertex,
        Fragment
    };
    
public:
    Shader() = delete;
    Shader(const std::string& shaderPath, Type shaderType, const std::shared_ptr<VkDevice_T>& device);
    ~Shader();

    VkPipelineShaderStageCreateInfo getShaderStageCreateInfo();

private:
    static std::vector<char> readFile(const std::string& filename);

    inline static const std::map<Type, VkShaderStageFlagBits> shaderStageFlags = {
            {Type::Vertex, VK_SHADER_STAGE_VERTEX_BIT},
            {Type::Fragment, VK_SHADER_STAGE_FRAGMENT_BIT}
    };

    void createShaderModule(const std::vector<char>& code);
    
    VkShaderModule shader = nullptr;
    Type type;
    
    std::shared_ptr<VkDevice_T> devicePtr = nullptr;
};
