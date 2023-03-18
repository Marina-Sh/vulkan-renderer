#include "Shader.h"

#include <fstream>


Shader::Shader(const std::string& shaderPath, Type shaderType, const std::shared_ptr<VkDevice_T>& device) {
    devicePtr = device;
    type = shaderType;

    const auto shaderCode = readFile(shaderPath);
    createShaderModule(shaderCode);
}

Shader::~Shader() {
    vkDestroyShaderModule(devicePtr.get(), shader, nullptr);
    shader = nullptr;
}

VkPipelineShaderStageCreateInfo Shader::getShaderStageCreateInfo() {
    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = shaderStageFlags.at(type);
    shaderStageInfo.module = shader;
    shaderStageInfo.pName = "main";

    return shaderStageInfo;
}

void Shader::createShaderModule(const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    
    if (vkCreateShaderModule(devicePtr.get(), &createInfo, nullptr, &shader) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
}

std::vector<char> Shader::readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }
    const std::streamsize fileSize =  file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    
    return buffer;
}
