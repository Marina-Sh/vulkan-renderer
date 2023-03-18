#include "HelloTriangleApplication.h"
#include "MeshLoader.h"


void HelloTriangleApplication::run() {
    window = std::make_shared<Window>();
    window->setResizeCallback(framebufferResizeCallback);
    window->drawFrame = [this](){drawFrame();};
    initVulkan();
    mainLoop();
    cleanup();
}

void HelloTriangleApplication::initVulkan() {
    instance = std::make_unique<Instance>();
    surface = std::make_unique<Surface>([windowPtr = window](VkInstance inst) {return windowPtr->createSurface(inst);}, instance->getHandle());
    physicalDevice = std::make_shared<PhysicalDevice>(instance->getHandle(), surface->getHandle());
    Instance::providePhysicalDevice(physicalDevice);
    
    device = std::make_unique<Device>(physicalDevice, instance->getValidationLayers());

    drawHelper = std::make_shared<DrawHelper>(device->getHandle());
    Instance::provideDrawHelper(drawHelper);

    mesh = MeshLoader::loadMesh(modelPath, device->getHandle());
    texture = std::make_shared<Texture>(device->getHandle(), texturePath);
    
    renderer = std::make_unique<Renderer>(device->getHandle(), window, surface->getHandle(), mesh, texture);
}

void HelloTriangleApplication::mainLoop() {
    window->run();
    device->waitIdle();
}

void HelloTriangleApplication::cleanup() {
}

void HelloTriangleApplication::drawFrame() {
    renderer->drawFrame();
}

void HelloTriangleApplication::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
    if (app->renderer != nullptr) {
        app->renderer->setFramebufferResized();
    }
}
