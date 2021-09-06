#include "GXVulkanSwapChain.h"
#include "Logging/Logger.h"

namespace gx {

    bool VulkanCreateSwapchain(GXVulkanContext* Context, GXVulkanSwapchain *OutSwapchain, GXuint32 Width, GXuint32 Height, VkPresentModeKHR PresentMode)
    {
        OutSwapchain->format = VK_FORMAT_B8G8R8A8_SRGB;
        OutSwapchain->present_mode = PresentMode;

        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Context->device.physical,
                                                  Context->surface,
                                                  &surfaceCapabilities);

        GXuint32 surfaceFormatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(Context->device.physical, Context->surface, &surfaceFormatCount, nullptr);
        std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(Context->device.physical, Context->surface, &surfaceFormatCount, surfaceFormats.data());

        GXuint32 presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(Context->device.physical, Context->surface, &presentModeCount, nullptr);
        std::vector<VkPresentModeKHR> presentModes(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(Context->device.physical, Context->surface, &presentModeCount, presentModes.data());

        // Todo(Harlequin): make sure the swapchain present mode is supported
        GXuint32 imageCount = surfaceCapabilities.minImageCount + 1;
        
        if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount)
        {
            imageCount = surfaceCapabilities.maxImageCount;
        }

        VkExtent2D extent;
        extent.width = Width;
        extent.height = Height;

        VkSwapchainCreateInfoKHR swapchainCreateInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
        swapchainCreateInfo.surface = Context->surface;
        swapchainCreateInfo.minImageCount = imageCount;
        swapchainCreateInfo.imageFormat = OutSwapchain->format;
        swapchainCreateInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        swapchainCreateInfo.imageExtent = extent;
        swapchainCreateInfo.imageArrayLayers = 1;
        swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        if (Context->device.queue_family_indices.graphics_queue_family_index != Context->device.queue_family_indices.present_queue_family_index)
        {
            GXuint32 queueFamilyIndicies[] = 
            {
                static_cast<GXuint32>(Context->device.queue_family_indices.graphics_queue_family_index),
                static_cast<GXuint32>(Context->device.queue_family_indices.present_queue_family_index)
            };

            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapchainCreateInfo.queueFamilyIndexCount = 2;
            swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndicies;
        }
        else
        {
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapchainCreateInfo.queueFamilyIndexCount = 0;
            swapchainCreateInfo.pQueueFamilyIndices = nullptr;
        }

        swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
        swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainCreateInfo.presentMode = PresentMode;
        swapchainCreateInfo.clipped = VK_TRUE;
        swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

        VkResult result = vkCreateSwapchainKHR(Context->device.logical, &swapchainCreateInfo, nullptr, &OutSwapchain->handle);

        if (result != VK_SUCCESS)
        {
            return false;
        }

        OutSwapchain->images.resize(imageCount);
        vkGetSwapchainImagesKHR(Context->device.logical, OutSwapchain->handle , &imageCount, OutSwapchain->images.data());

        OutSwapchain->image_views.resize(imageCount);
        
        for (GXuint32 i = 0; i < imageCount; i++) 
        {
            VkImageViewCreateInfo imageViewCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
            imageViewCreateInfo.image = OutSwapchain->images[i];
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.format = OutSwapchain->format;
            imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            imageViewCreateInfo.subresourceRange.levelCount = 1;
            imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            imageViewCreateInfo.subresourceRange.layerCount = 1;

            VkResult result = vkCreateImageView(Context->device.logical, &imageViewCreateInfo, nullptr, &OutSwapchain->image_views[i]);
            
            if (result != VK_SUCCESS)
            {
                return false;
            }
        }

        return true;
    }

    void VulkanDestroySwapchain(GXVulkanContext *Context, GXVulkanSwapchain *Swapchain)
    {
        for (GXuint32 i = 0; i < Swapchain->images.size(); i++)
        {
            vkDestroyImageView(Context->device.logical, Swapchain->image_views[i], nullptr);
        }

        vkDestroySwapchainKHR(Context->device.logical, Swapchain->handle, nullptr);
    }

    bool VulkanRecreateSwapchain(GXVulkanContext* Context, GXVulkanSwapchain* Swapchain, GXuint32 Width, GXuint32 Height, VkPresentModeKHR PresentMode)
    {
        VulkanDestroySwapchain(Context, Swapchain);
        bool isRecreationSuccessful = VulkanCreateSwapchain(Context, Swapchain, Width, Height, PresentMode);
        return isRecreationSuccessful;
    }

    bool VulkanAcquireNextImageIndex(GXVulkanContext* Context, GXVulkanSwapchain* Swapchain, VkSemaphore ImageAvailable, GXuint32 *OutImageIndex)
    {
        VkResult result = vkAcquireNextImageKHR(Context->device.logical, Swapchain->handle, 1000000000, ImageAvailable, nullptr, OutImageIndex);
        
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            bool isRecreationSuccessful = VulkanRecreateSwapchain(Context, Swapchain, Context->framebuffer_width, Context->framebuffer_height, Swapchain->present_mode);
            
            if (!isRecreationSuccessful)
            {
                return false;
            }
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            GXE_ERROR("failed to acquire swapchain image index.");
            return false;
        }

        return true;
    }
}