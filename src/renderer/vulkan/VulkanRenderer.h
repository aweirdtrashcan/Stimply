#pragma once

#include "renderer/Renderer.h"
#include "core/utils/Includes.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#define VK_CHECK(res) if (res != VK_SUCCESS) { Logger::SLOG("VK_CHECK Failed at: '%s' at File '%s', line '%i'", #res, __FILE__, __LINE__); debugBreak(); }

class VulkanRenderer : public Renderer
{
public:
    VulkanRenderer() = default;
    virtual ~VulkanRenderer() override;

    virtual bool BeginFrame(float deltaTime) override;
    virtual bool EndFrame(float deltaTime) override;
    virtual void Resize(int width, int height) override;
    virtual void Initialize(const char* rendererName, const char* applicationName, int width, int height, struct GLFWwindow* window) override;

private:
    struct GLFWwindow* window = nullptr;

    VkInstance instance = nullptr;
    VkAllocationCallbacks* allocator = nullptr;
    VkDebugUtilsMessengerEXT messenger = nullptr;
    VkSurfaceKHR surface = nullptr;
    VkPhysicalDevice physicalDevice = nullptr;
    VkDevice logicalDevice = nullptr;

private:
    void createInstance(const char* rendererName, const char* applicationName);
    void createVulkanDebugger(PFN_vkDebugUtilsMessengerCallbackEXT callback);
    void destroyVulkanDebugger();
    static VkBool32 VKAPI_PTR debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
        const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
        void*                                            pUserData
    );
};