#pragma once

#include "renderer/Renderer.h"
#include "core/utils/Includes.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#define VK_CHECK(res) if (res != VK_SUCCESS) { Logger::SLOG("VK_CHECK Failed at: '%s' at File '%s', line '%i'", #res, __FILE__, __LINE__); debugBreak(); }

class VulkanRenderer : public Renderer
{
public:
    VulkanRenderer(const char* rendererName, const char* applicationName, int width, int height);
    virtual ~VulkanRenderer() override;

    virtual bool BeginFrame(float deltaTime) override;
    virtual bool EndFrame(float deltaTime) override;
    virtual void Resize(int width, int height) override;

private:
    VkInstance instance = nullptr;
    VkDebugUtilsMessengerEXT messenger = nullptr;
    VkPhysicalDevice physicalDevice = nullptr;
    VkDevice logicalDevice = nullptr;

private:
    void createInstance(const char* rendererName, const char* applicationName);
};