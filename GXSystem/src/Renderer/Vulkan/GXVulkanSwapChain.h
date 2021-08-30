#ifndef GX_VULKAN_SWAP_CHAIN_H_
#define GX_VULKAN_SWAP_CHAIN_H_

#include "GXVulkanTypes.h"

namespace gx {
    bool VulkanCreateSwapchain(GXVulkanContext* Context, GXVulkanSwapchain *OutSwapchain, GXuint32 Width, GXuint32 Height, VkPresentModeKHR PresentMode);
    void VulkanDestroySwapchain(GXVulkanContext* Context, GXVulkanSwapchain *Swapchain);
    bool VulkanRecreateSwapchain(GXVulkanContext* Context, GXVulkanSwapchain* Swapchain, GXuint32 Width, GXuint32 Height, VkPresentModeKHR PresentMode);
    bool VulkanAcquireNextImageIndex(GXVulkanContext* Context, GXVulkanSwapchain* Swapchain, GXuint64 TimeoutNS, VkSemaphore ImageAvailable, VkFence Fence, GXuint32 *OutImageIndex);
}

#endif