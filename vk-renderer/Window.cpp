#include "Window.h"

#include <stdexcept>

Window::Window() {
    init();
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate(); 
}

void Window::init() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    
    window = glfwCreateWindow(defaultSize.width, defaultSize.height, "Vulkan", nullptr, nullptr);
    
    glfwSetWindowUserPointer(window, this);
}

void Window::setResizeCallback(GLFWframebuffersizefun callback) {
    glfwSetFramebufferSizeCallback(window, callback);
}

ImageSize Window::getFramebufferSize() {
    int width;
    int height;
    glfwGetFramebufferSize(window, &width, &height);
    return ImageSize{width, height};
}

VkSurfaceKHR Window::createSurface(VkInstance instance) {
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }

    return surface;
}

void Window::run() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if(drawFrame) {
            drawFrame();
        }
    }
}
