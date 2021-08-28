#ifndef GX_VULKAN_TYPES_H_
#define GX_VULKAN_TYPES_H_

#include <vulkan/vulkan.h>

struct GXVulkanContext {
    VkInstance instance;
    VkSurfaceKHR surface;

    VkDebugUtilsMessengerEXT debug_messenger;
};

#endif // GX_VULKAN_TYPES_H_