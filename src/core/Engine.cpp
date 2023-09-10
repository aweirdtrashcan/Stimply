#include "Engine.h"
#include "core/utils/Includes.h"
#include "renderer/vulkan/VulkanRenderer.h"

#include <GLFW/glfw3.h>

Engine::Engine(const char* applicationName, int width, int height)
    :
    applicationName(applicationName),
    width(width),
    height(height),
    renderer("Stimply Renderer", applicationName, width, height)
{
    if (glfwInit() != GLFW_TRUE)
    {
        Logger::SLOG("Failed to initialize GLFW.");
        debugBreak();
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    if (!(window = glfwCreateWindow(width, height, applicationName, nullptr, nullptr))) 
    {
        Logger::SLOG("Failed to create GLFW window.");
        debugBreak();
    }
}

Engine::~Engine()
{
    glfwDestroyWindow(window);
    window = 0;
    applicationName = 0;
    window = 0;
    height = 0;
}

void Engine::Run()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        renderer.BeginFrame(0);
        renderer.EndFrame(0);

        glfwSwapBuffers(window);
    }
}