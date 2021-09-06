#ifndef GX_VULKAN_RENDER_PASS_H_
#define GX_VULKAN_RENDER_PASS_H_

#include "GXVulkanTypes.h"

namespace gx {

    bool VulkanCreateRenderPass(GXVulkanContext* Context, VkRenderPass* OutRenderPass);
    void VulkanDestroyRenderPass(GXVulkanContext* Context, VkRenderPass* RenderPass);

    bool VulkanCreateFrameBuffer(GXVulkanContext* Context, VkRenderPass* RenderPass, VkImageView *ImageView, VkFramebuffer *OutFrameBuffer);
    void VulkanDestroyFrameBuffer(GXVulkanContext* Context, VkFramebuffer *FrameBuffer);
}

#endif