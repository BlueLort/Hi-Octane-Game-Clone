#ifndef GX_VULKAN_TYPES_H_
#define GX_VULKAN_TYPES_H_

#include "GX/GXTypes.h"
#include <vulkan/vulkan.h>

struct GXVulkanQueueFamilyIndices {
    GXint32 graphics_queue_family_index;
    GXint32 present_queue_family_index;
    GXint32 compute_queue_family_index;
    GXint32 transfer_queue_family_index;
};

struct GXVulkanDevice {
    VkPhysicalDevice physical;
    VkDevice         logical;

    VkPhysicalDeviceProperties       properties;
    VkPhysicalDeviceFeatures         features;
    VkPhysicalDeviceMemoryProperties memory;

    GXVulkanQueueFamilyIndices queue_family_indices;

    VkQueue graphics_queue;
    VkQueue compute_queue;
    VkQueue present_queue;
    VkQueue transfer_queue;
};

struct GXVulkanContext {
    VkInstance instance;
    VkSurfaceKHR surface;
    GXVulkanDevice device;

    VkDebugUtilsMessengerEXT debug_messenger;
};

#endif // GX_VULKAN_TYPES_H_