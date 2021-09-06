#include "GXVulkanCommandBuffers.h"

namespace gx {

    bool VulkanCreateCommandPool(GXVulkanContext* Context, GXuint32 QueueFamilyIndex, VkCommandPool* OutCommandPool)
    {
        VkCommandPoolCreateInfo commandPoolCreateInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
        commandPoolCreateInfo.queueFamilyIndex = QueueFamilyIndex;
        commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        VkResult result = vkCreateCommandPool(Context->device.logical, &commandPoolCreateInfo, nullptr, OutCommandPool);

        if (result != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    bool VulkanAllocateCommandBuffer(GXVulkanContext* Context, VkCommandPool* CommandPool, VkCommandBuffer* OutCommandBuffer)
    {
        VkCommandBufferAllocateInfo commandBufferAllocateInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
        commandBufferAllocateInfo.commandPool = *CommandPool;
        commandBufferAllocateInfo.commandBufferCount = 1;
        commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

        VkResult result = vkAllocateCommandBuffers(Context->device.logical, &commandBufferAllocateInfo, OutCommandBuffer);

        if (result != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }
    
    void VulkanDestroyCommandPool(GXVulkanContext* Context, VkCommandPool* CommandPool)
    {
        vkDestroyCommandPool(Context->device.logical, *CommandPool, nullptr);
    }
}