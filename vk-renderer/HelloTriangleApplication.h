#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Instance.h"
#include "Window.h"
#include "Surface.h"
#include "PhysicalDevice.h"
#include "Device.h"
#include "Renderer.h"
#include "Helpers.h"
#include "Mesh.h"
#include "Texture.h"
#include "Vertex.h"

class HelloTriangleApplication {
    
    std::shared_ptr<Window> window = nullptr;
    std::unique_ptr<Instance> instance = nullptr;
    std::shared_ptr<PhysicalDevice> physicalDevice = nullptr;
    std::unique_ptr<Device> device = nullptr;
    std::unique_ptr<Surface> surface = nullptr;
    
    std::shared_ptr<DrawHelper> drawHelper = nullptr;
    
    std::shared_ptr<Texture> texture = nullptr;
    std::shared_ptr<Mesh> mesh = nullptr;
    
    std::unique_ptr<Renderer> renderer = nullptr;

    void drawFrame();
    
    const std::vector<Vertex> triangleVertices = {
        {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.f, 0.f} },
        {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.f, 0.f} },
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.f, 0.f} }
    };

    const std::vector<uint32_t> triangleIndices = {
        0, 1, 2 
    };

    const std::vector<Vertex> rectangleVertices = {
        {{-0.5f, -0.5f, 0.f}, {1.0f, 0.0f, 0.0f} , {1.f, 0.f} },
        {{0.5f, -0.5f, 0.f}, {0.0f, 1.0f, 0.0f}, {0.f, 0.f} },
        {{0.5f, 0.5f, 0.f}, {0.0f, 0.0f, 1.0f}, {0.f, 1.f} },
        {{-0.5f, 0.5f, 0.f}, {1.0f, 1.0f, 1.0f},{1.f, 1.f} }
    };

    const std::vector<uint32_t> rectangleIndices = {
        0, 1, 2, 2, 3, 0
    };

    const std::vector<Vertex> depthTextureVertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };

    const std::vector<uint32_t> depthTextureIndices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };

    const std::string modelPath = "models/viking_room.obj";
    const std::string texturePath = "textures/viking_room.png";
    
public:
    void run();

private:
    void initVulkan();
    void mainLoop();
    void cleanup();
    
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};
