#pragma once

#include "utils/Includes.h"
#include "renderer/vulkan/VulkanRenderer.h"

class Engine
{
public:
    Engine(const char* applicationName, int width, int height);
    ~Engine();

    void Run();

private:
    const char* applicationName = nullptr;
    int width = -1;
    int height = -1;

    struct GLFWwindow* window = nullptr;

    VulkanRenderer renderer;
};