#include "GXVulkanBuffers.h"

namespace gx {

    bool VulkanCreateVertexBuffer(GXVulkanContext *Context, GXFloat *Vertices, GXuint64 size, GXVulkanVertexBuffer *OutVertexBuffer)
    {
        VkBufferCreateInfo bufferCreateInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
        bufferCreateInfo.size = size;
        bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

        VmaAllocationCreateInfo allocationCreateInfo = {};
        allocationCreateInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

        VkBuffer buffer;
        VmaAllocation allocation;

        if (vmaCreateBuffer(Context->allocator, &bufferCreateInfo, &allocationCreateInfo, &buffer, &allocation, nullptr) != VK_SUCCESS)
        {
            OutVertexBuffer = nullptr;
            return false;
        }

        void *data;
        vmaMapMemory(Context->allocator, allocation, &data);
        memcpy(data, Vertices, size);
        vmaUnmapMemory(Context->allocator, allocation);

        OutVertexBuffer->buffer = buffer;
        OutVertexBuffer->allocation = allocation;

        return true;
    }

    void VulkanDestroyVertexBuffer(GXVulkanContext *Context, GXVulkanVertexBuffer *VertexBuffer)
    {
    }
}