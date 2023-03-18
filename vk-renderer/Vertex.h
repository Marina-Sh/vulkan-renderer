#pragma once

#include <array>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

struct Vertex {

    static constexpr int attributesNum = 3; 

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, attributesNum> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, attributesNum> attributeDescriptions{};

        for (int i = 0; i < attributesNum; i++) {
            attributeDescriptions[i].binding = 0;
            attributeDescriptions[i].location = i;
            attributeDescriptions[i].format = VK_FORMAT_R32G32B32_SFLOAT;
        }

        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;

        attributeDescriptions[0].offset = offsetof(Vertex, pos);
        attributeDescriptions[1].offset = offsetof(Vertex, color);
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
        
        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }

    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
};


template<> struct std::hash<Vertex> {
    size_t operator()(Vertex const& vertex) const {
        return ((std::hash<glm::vec3>()(vertex.pos) ^
               (std::hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
               (std::hash<glm::vec2>()(vertex.texCoord) << 1);
    }
};