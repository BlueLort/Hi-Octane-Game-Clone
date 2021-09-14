#ifndef GX_VULKAN_BUFFERS_H_
#define GX_VULKAN_BUFFERS_H_

#include "GXVulkanTypes.h"

namespace gx {

    bool VulkanCreateVertexBuffer(GXVulkanContext *Context, GXFloat *Vertices, GXuint64 size, GXVulkanVertexBuffer *OutVertexBuffer);

    void VulkanDestroyVertexBuffer(GXVulkanContext *Context, GXVulkanVertexBuffer *VertexBuffer);
}

#endif // GX_VULKAN_BUFFERS_H_