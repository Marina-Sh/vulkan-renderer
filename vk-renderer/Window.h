#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <functional>

struct ImageSize {
    int width;
    int height;
};

class Window {
public:
    Window();
    ~Window();

    void setResizeCallback(GLFWframebuffersizefun callback);
    
    ImageSize getFramebufferSize();
    VkSurfaceKHR createSurface(VkInstance instance);

    void run();
    
    std::function<void()> drawFrame;
private:
    void init();

    const ImageSize defaultSize {800, 600};
    
    GLFWwindow* window = nullptr;
};
