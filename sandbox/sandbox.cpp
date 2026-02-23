#include "toy2d/toy2d.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
// If you have selected SDL2 component when installed Vulkan SDK
// The following codes will work
// #include <SDL2/SDL.h>
// #include <SDL2/SDL_vulkan.h>
// #include <SDL2/SDL_video.h>

constexpr uint32_t WindowWidth = 1024;
constexpr uint32_t WindowHeight = 720;

int main(int argc, char **argv)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow *window = glfwCreateWindow(WindowWidth, WindowHeight, "Vulkan", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Window create failed!\n";
        exit(2);
    }

    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    if (glfwExtensions == nullptr)
    {
        throw std::runtime_error("Error: no extensions support");
    }
    std::vector<const char *> extensions = {glfwExtensions, glfwExtensions + glfwExtensionCount};

#ifdef __APPLE__
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif

    toy2d::Init(
        extensions,
        [&](VkInstance instance) {
            VkSurfaceKHR surface;
            if (glfwCreateWindowSurface(instance, window, nullptr, &surface))
                throw std::runtime_error("glf create window surface failed!");
            return surface;
        },
        1024, 720);
    auto renderer = toy2d::GetRenderer();

    bool shouldClose = false;

    float x = 100, y = 100;

    toy2d::Texture *texture1 = toy2d::LoadTexture("resources/role.png");
    toy2d::Texture *texture2 = toy2d::LoadTexture("resources/texture.jpg");

    while (glfwWindowShouldClose(window) == 0)
    {
        glfwPollEvents();

        renderer->StartRender();
        renderer->SetDrawColor(toy2d::Color{1, 0, 0});
        renderer->DrawTexture(toy2d::Rect{toy2d::Vec{x, y}, toy2d::Size{200, 300}}, *texture1);
        renderer->SetDrawColor(toy2d::Color{0, 1, 0});
        renderer->DrawTexture(toy2d::Rect{toy2d::Vec{500, 100}, toy2d::Size{200, 300}}, *texture2);
        renderer->SetDrawColor(toy2d::Color{0, 0, 1});
        renderer->DrawLine(toy2d::Vec{0, 0}, toy2d::Vec{WindowWidth, WindowHeight});
        renderer->EndRender();
    }

    toy2d::DestroyTexture(texture1);
    toy2d::DestroyTexture(texture2);

    toy2d::Quit();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
