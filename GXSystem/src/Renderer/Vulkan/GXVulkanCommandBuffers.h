#ifndef GX_VULKAN_COMMAND_BUFFERS_H_
#define GX_VULKAN_COMMAND_BUFFERS_H_

#include "GXVulkanTypes.h"

namespace gx {
    bool VulkanCreateCommandPool(GXVulkanContext *Context, GXuint32 QueueFamilyIndex, VkCommandPool *OutCommandPool);
    bool VulkanAllocateCommandBuffer(GXVulkanContext* Context, VkCommandPool* CommandPool, VkCommandBuffer* OutCommandBuffer);
    void VulkanDestroyCommandPool(GXVulkanContext* Context, VkCommandPool* CommandPool);
}

#endif