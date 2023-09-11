#include "VulkanRenderer.h"
#include "core/utils/Includes.h"
#include "core/utils/logger.h"
#include <GLFW/glfw3.h>
#include <cstring>
#include <vector>
#include <vulkan/vulkan_core.h>

void VulkanRenderer::Initialize(const char* rendererName, const char* applicationName, int width, int height, struct GLFWwindow* window) 
{
    Logger::SLOG("Initializing Vulkan Renderer.");
    createInstance(rendererName, applicationName);
    createVulkanDebugger(&VulkanRenderer::debugCallback);
    VK_CHECK(glfwCreateWindowSurface(instance, window, allocator, &surface));
    SINFO("Vulkan Surface created successfully.");
}

VulkanRenderer::~VulkanRenderer()
{
    Logger::SLOG("Destroying Vulkan Surface.");
    vkDestroySurfaceKHR(instance, surface, allocator);
    Logger::SLOG("Destroying Vulkan Debugger.");
    destroyVulkanDebugger();
    Logger::SLOG("Destroying Vulkan Instance.");
    vkDestroyInstance(instance, allocator);
}

bool VulkanRenderer::BeginFrame(float deltaTime)
{
    return true;
}

bool VulkanRenderer::EndFrame(float deltaTime)
{
    return true;
}

void VulkanRenderer::Resize(int width, int height)
{

}

/* PRIVATE FUNCTIONS */

void VulkanRenderer::createInstance(const char* rendererName, const char* applicationName)
{
    VkApplicationInfo applicationInfo = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    const uint32_t appVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    applicationInfo.applicationVersion = appVersion;
    applicationInfo.engineVersion = appVersion;
    applicationInfo.apiVersion = VK_MAKE_API_VERSION(0, 1, 2, 0);
    applicationInfo.pEngineName = rendererName;
    applicationInfo.pNext = nullptr;
    applicationInfo.pApplicationName = applicationName;

    VkInstanceCreateInfo instanceInfo = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    instanceInfo.pApplicationInfo = &applicationInfo;

    uint32_t propertyCount = 0;
    VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr));
    if (propertyCount == 0)
    {
        SFATAL("Failed to enumerate instance extensions.");
    }
    std::vector<VkExtensionProperties> extensionProperties(propertyCount);
    VK_CHECK(vkEnumerateInstanceExtensionProperties(0, &propertyCount, extensionProperties.data()));

    std::vector<const char*> requiredExtensions;

#ifdef _DEBUG
    SINFO("Supported instance extensions:");
    for (VkExtensionProperties& p : extensionProperties) 
    {
        SINFO("  %s", p.extensionName);
    }
    requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    requiredExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    //requiredExtensions.push_back("VK_KHR_xlib_surface");
    requiredExtensions.push_back("VK_KHR_xcb_surface");
    // yes. hardcoded bc im making this on linux and am too lazy.

    for (const char* req : requiredExtensions)
    {
        bool found = false;
        for (VkExtensionProperties& prop : extensionProperties)
        {
            if (strcmp(prop.extensionName, req) == 0)
            {
                SINFO("Extension: %s is supported by the Instance.", req);
                found = true;
                break;
            }
        }
        
        if (!found)
        {
            SFATAL("Extension: %s is not supported by the Instance. Leaving application.", req);
        }
    }

    instanceInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    instanceInfo.ppEnabledExtensionNames = requiredExtensions.data();

    // Setting to 0 so i can re-use.
    propertyCount = 0;
    VK_CHECK(vkEnumerateInstanceLayerProperties(&propertyCount, nullptr));
    if (propertyCount == 0)
    {
        SFATAL("Failed to enumerate instance layers.");
    }
    std::vector<VkLayerProperties> layerProperties(propertyCount);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&propertyCount, layerProperties.data()));

    std::vector<const char*> requiredLayers;

#ifdef _DEBUG
    SINFO("Supported instance layers:");
    for (VkLayerProperties& p : layerProperties)
    {
        SINFO("  %s", p.layerName);
    }
    requiredLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif

    for (const char* req : requiredLayers)
    {
        bool found = false;
        for (VkLayerProperties& p : layerProperties)
        {
            if (strcmp(req, p.layerName) == 0)
            {
                SINFO("Layer: %s is supported by the Instance.", req);
                found = true;
                break;
            }
        }

        if (!found)
        {
            SFATAL("Layer: %s is not supported by the Instance. Leaving Application.", req);
        }
    }

    instanceInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
    instanceInfo.ppEnabledLayerNames = requiredLayers.data();

    VK_CHECK(vkCreateInstance(&instanceInfo, allocator, &instance));

    SINFO("Vulkan Instance created successfully.");
}

void VulkanRenderer::createVulkanDebugger(PFN_vkDebugUtilsMessengerCallbackEXT callback)
{
#ifdef _DEBUG
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
    createInfo.pfnUserCallback = callback;

    PFN_vkCreateDebugUtilsMessengerEXT func = nullptr;
    func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));

    if (!func)
    {
        SFATAL("Failed to load vkCreateDebugUtilsMessengerEXT function pointer.");
    }

    VK_CHECK(func(instance, &createInfo, allocator, &messenger));

    SINFO("Created Vulkan Debugger successfully.");
#endif
}

void VulkanRenderer::destroyVulkanDebugger()
{
#ifdef _DEBUG
    PFN_vkDestroyDebugUtilsMessengerEXT func = nullptr;
    func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));

    if (!func)
    {
        SFATAL("Failed to load vkDestroyDebugUtilsMessengerEXT function pointer.");
    }

    func(instance, messenger, allocator);
#endif
}

VkBool32 VKAPI_PTR VulkanRenderer::debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
        const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
        void*                                            pUserData
) {
    SINFO("%s", pCallbackData->pMessage);
    return VK_FALSE;
}