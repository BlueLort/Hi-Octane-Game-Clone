#ifndef GX_VULKAN_TYPES_H_
#define GX_VULKAN_TYPES_H_

#include "GX/GXTypes.h"

#include <vector>
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

struct GXVulkanSwapchain {
    VkSwapchainKHR handle;
    VkFormat format;
    VkPresentModeKHR present_mode;
    std::vector<VkImage> images;
    std::vector<VkImageView> image_views;
};

struct GXVulkanPipeline {
    VkPipeline handle;
    VkPipelineLayout layout;
};

struct GXVulkanContext {
    VkInstance instance;
    VkSurfaceKHR surface;
    GXVulkanDevice device;
    GXVulkanSwapchain swapchain;

    VkCommandPool graphics_pool;
    VkCommandBuffer graphics_buffer;

    VkRenderPass render_pass;

    GXuint32 framebuffer_width;
    GXuint32 framebuffer_height;

    std::vector<VkFramebuffer> frame_buffers;

    VkFence render_fence;
    VkSemaphore render_semaphore;
    VkSemaphore present_semaphore;

    GXuint32 swapchain_image_index;

    GXVulkanPipeline pipeline;

    VkShaderModule vertex_shader;
    VkShaderModule fragment_shader;

    VkDebugUtilsMessengerEXT debug_messenger;
};

#endif // GX_VULKAN_TYPES_H_