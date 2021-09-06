#include "GXVulkanRenderPass.h"

namespace gx {

    bool VulkanCreateRenderPass(GXVulkanContext* Context, VkRenderPass* OutRenderPass)
    { 
        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = Context->swapchain.format;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassCreateInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
        renderPassCreateInfo.attachmentCount = 1;
        renderPassCreateInfo.pAttachments = &colorAttachment;

        renderPassCreateInfo.subpassCount = 1;
        renderPassCreateInfo.pSubpasses = &subpass;

        VkResult result = vkCreateRenderPass(Context->device.logical, &renderPassCreateInfo, nullptr, OutRenderPass);
        
        if (result != VK_SUCCESS) 
        {
            return false;
        }

        return true;
    }

    void VulkanDestroyRenderPass(GXVulkanContext* Context, VkRenderPass* RenderPass)
    {
        vkDestroyRenderPass(Context->device.logical, *RenderPass, nullptr);
    }


    bool VulkanCreateFrameBuffer(GXVulkanContext* Context, VkRenderPass* RenderPass, VkImageView* ImageView, VkFramebuffer* OutFrameBuffer)
    {
        VkFramebufferCreateInfo frameBufferCreateInfo = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
        frameBufferCreateInfo.renderPass = *RenderPass;
        frameBufferCreateInfo.attachmentCount = 1;
        frameBufferCreateInfo.width = Context->framebuffer_width;
        frameBufferCreateInfo.height = Context->framebuffer_height;
        frameBufferCreateInfo.layers = 1;
        frameBufferCreateInfo.pAttachments = ImageView;

        VkResult result = vkCreateFramebuffer(Context->device.logical, &frameBufferCreateInfo, nullptr, OutFrameBuffer);
        
        if (result != VK_SUCCESS) 
        {
            return false;
        }

        return true;
    }

    void VulkanDestroyFrameBuffer(GXVulkanContext* Context, VkFramebuffer* FrameBuffer)
    {
        vkDestroyFramebuffer(Context->device.logical, *FrameBuffer, nullptr);
    }
}