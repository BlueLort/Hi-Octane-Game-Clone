#include "GXVulkanRenderer.h"
#include "GXVulkanTypes.h"
#include "GXVulkanDevice.h"
#include "GXVulkanSwapChain.h"
#include "GXVulkanRenderPass.h"
#include "GXVulkanCommandBuffers.h"
#include "GXVulkanShader.h"
#include "GXVulkanPipeline.h"

#include "Logging/Logger.h"
#include "Platform/GXWindow.h"

#include <SDL/SDL_Vulkan.h>

namespace gx {
    
    static GXVulkanContext context;
    static GXuint32 cached_width;
    static GXuint32 cached_height;

    static VkBool32 VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data);
    
    static void RecreateSwapchain()
    {
        VulkanRecreateSwapchain(&context, &context.swapchain, cached_width, cached_height, context.swapchain.present_mode);

        for (GXuint32 i = 0; i < context.frame_buffers.size(); i++)
        {
            VulkanDestroyFrameBuffer(&context, &context.frame_buffers[i]);
            VulkanCreateFrameBuffer(&context, &context.render_pass, &context.swapchain.image_views[i], &context.frame_buffers[i]);
        }
    }

    static void RecreatePipeline()
    {
        VulkanDestroyPipeline(&context, &context.pipeline);

        GXPipelineCreateInfo pipelineCreateInfo;
        pipelineCreateInfo.vertex_shader   = &context.vertex_shader;
        pipelineCreateInfo.fragment_shader = &context.fragment_shader;

        VulkanCreatePipeline(&context, &pipelineCreateInfo, &context.pipeline);
    }

    static void VulkanBeginFrame()
    {
        vkWaitForFences(context.device.logical, 1, &context.render_fence, true, 1000000000);
        vkResetFences(context.device.logical, 1, &context.render_fence);

        if ((cached_width != context.framebuffer_width || cached_height != context.framebuffer_height) && cached_width != 0 && cached_height != 0)
        {
            context.framebuffer_width = cached_width;
            context.framebuffer_height = cached_height;

            RecreateSwapchain();
            RecreatePipeline();
        }

        VulkanAcquireNextImageIndex(&context, &context.swapchain, context.present_semaphore, &context.swapchain_image_index);

        vkResetCommandBuffer(context.graphics_buffer, 0);
        VkCommandBufferBeginInfo cmdBufferBeginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
        cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(context.graphics_buffer, &cmdBufferBeginInfo);

        VkClearValue clearValue;
        clearValue.color = { 1.0f, 0.0f, 0.0f, 1.0f };

        VkRenderPassBeginInfo renderPassBeginInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
        renderPassBeginInfo.renderPass = context.render_pass;
        renderPassBeginInfo.renderArea.offset.x = 0;
        renderPassBeginInfo.renderArea.offset.y = 0;
        renderPassBeginInfo.renderArea.extent.width = context.framebuffer_width;
        renderPassBeginInfo.renderArea.extent.height = context.framebuffer_height;
        renderPassBeginInfo.framebuffer = context.frame_buffers[context.swapchain_image_index];
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearValue;

        vkCmdBeginRenderPass(context.graphics_buffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(context.graphics_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, context.pipeline.handle);
        vkCmdDraw(context.graphics_buffer, 3, 1, 0, 0);
    }

    static void VulkanEndFrame()
    {
        vkCmdEndRenderPass(context.graphics_buffer);
        vkEndCommandBuffer(context.graphics_buffer);

        VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
        
        VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        submitInfo.pWaitDstStageMask = &waitStage;

        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &context.present_semaphore;

        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &context.render_semaphore;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &context.graphics_buffer;

        vkQueueSubmit(context.device.graphics_queue, 1, &submitInfo, context.render_fence);

        VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &context.swapchain.handle;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &context.render_semaphore;
        presentInfo.pImageIndices = &context.swapchain_image_index;

        vkQueuePresentKHR(context.device.graphics_queue, &presentInfo);
    }

    static void VulkanOnResize(GXuint32 Width, GXuint32 Height)
    {
        cached_width = Width;
        cached_height = Height;
    }

    bool VulkanInit(GXRendererAPIFunctions* ApiFunctions)
    {
        VkApplicationInfo appInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
        appInfo.pApplicationName = "Octaine";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "GXSystem";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_2;

        SDL_Window* window = GXWindow::GetSDLWindow();

        GXuint32 extensionCount;
        if (!SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr))
        {
            return false;
        }

        std::vector<const char*> instanceExtensions(extensionCount);
        SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, instanceExtensions.data());

        std::vector<const char*> validationLayers;

#if GX_VULKAN_DEBUG
        instanceExtensions.push_back("VK_EXT_debug_utils");
        validationLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif // GX_VULKAN_DEBUG

        VkInstanceCreateInfo instanceCreateInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
        instanceCreateInfo.pApplicationInfo = &appInfo;

        instanceCreateInfo.enabledLayerCount = static_cast<GXuint32>(validationLayers.size());
        instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();

        instanceCreateInfo.enabledExtensionCount = static_cast<GXuint32>(instanceExtensions.size());
        instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();

        if (vkCreateInstance(&instanceCreateInfo, nullptr, &context.instance) != VK_SUCCESS)
        {
            GXE_ERROR("failed to create vulkan instance.");
            return false;
        }

#if GX_VULKAN_DEBUG

        VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
        debugMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugMessengerCreateInfo.pfnUserCallback = VulkanDebugCallback;
        debugMessengerCreateInfo.pUserData = nullptr;

        auto createDebugUtilsMessenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkCreateDebugUtilsMessengerEXT");

        if (createDebugUtilsMessenger(context.instance, &debugMessengerCreateInfo, nullptr, &context.debug_messenger) != VK_SUCCESS)
        {
            GXE_ERROR("failed to create debug messenger.");
            return false;
        }

#endif // GX_VULKAN_DEBUG

        if (!SDL_Vulkan_CreateSurface(window, context.instance, &context.surface))
        {
            GXE_ERROR("failed to vulkan surface.");
            return false;
        }

        GXVulkanDeviceRequirements requirements;
        
        requirements.has_graphics_queue = true;
        requirements.has_present_queue  = true;
        requirements.has_compute_queue  = true;
        requirements.has_transfer_queue = true;
        requirements.has_sampler_anisotropy = true;
        requirements.is_discrete = true;
        requirements.extensions = { "VK_KHR_swapchain" };

        if (!VulkanCreateDevice(&context, &requirements, &context.device))
        {
            GXE_ERROR("failed to create vulkan device.");
            return false;
        }

        GXint32 windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        context.framebuffer_width = windowWidth;
        context.framebuffer_height = windowHeight;
        cached_width = windowWidth;
        cached_height = windowHeight;

        // Todo:(Harlequin): v-sync for now we need to support triple buffering
        VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
        if (!VulkanCreateSwapchain(&context, &context.swapchain, windowWidth, windowHeight, presentMode))
        {
            GXE_ERROR("failed to create vulkan swapchain.");
            return false;
        }

        if (!VulkanCreateRenderPass(&context, &context.render_pass))
        {
            GXE_ERROR("failed to create vulkan render pass.");
            return false;
        }

        auto imageCount = static_cast<GXuint32>(context.swapchain.images.size());
        context.frame_buffers.resize(imageCount);

        for (GXuint32 i = 0; i < imageCount; i++)
        {
            if (!VulkanCreateFrameBuffer(&context, &context.render_pass, &context.swapchain.image_views[i], &context.frame_buffers[i]))
            {
                GXE_ERROR("failed to create vulkan framebuffer.");
                return false;
            }
        }

        if (!VulkanCreateCommandPool(&context, context.device.queue_family_indices.graphics_queue_family_index, &context.graphics_pool))
        {
            GXE_ERROR("failed to create graphics command pool.");
            return false;
        }

        if (!VulkanAllocateCommandBuffer(&context, &context.graphics_pool, &context.graphics_buffer))
        {
            GXE_ERROR("failed to create graphics command buffer.");
            return false;
        }

        VkFenceCreateInfo fenceCreateInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        VkResult result = vkCreateFence(context.device.logical, &fenceCreateInfo, nullptr, &context.render_fence);

        if (result != VK_SUCCESS)
        {
            GXE_ERROR("failed to create render fence.");
            return false;
        }

        VkSemaphoreCreateInfo semaphoreCreateInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
        semaphoreCreateInfo.flags = 0;

        result = vkCreateSemaphore(context.device.logical, &semaphoreCreateInfo, nullptr, &context.render_semaphore);

        if (result != VK_SUCCESS)
        {
            GXE_ERROR("failed to create render semaphore.");
            return false;
        }

        result = vkCreateSemaphore(context.device.logical, &semaphoreCreateInfo, nullptr, &context.present_semaphore);

        if (result != VK_SUCCESS)
        {
            GXE_ERROR("failed to create present semaphore.");
            return false;
        }

        GXE_INFO("Vulkan Renderer Initialized Successfully.");

        if (!VulkanLoadShaderModule(&context, "Assets/Shaders/Compiled/triangle.vert.spv", &context.vertex_shader))
        {
            return false;
        }

        if (!VulkanLoadShaderModule(&context, "Assets/Shaders/Compiled/triangle.frag.spv", &context.fragment_shader))
        {
            return false;
        }

        GXPipelineCreateInfo pipelineCreateInfo;
        pipelineCreateInfo.vertex_shader   = &context.vertex_shader;
        pipelineCreateInfo.fragment_shader = &context.fragment_shader;

        if (!VulkanCreatePipeline(&context, &pipelineCreateInfo, &context.pipeline))
        {
            return false;
        }

        ApiFunctions->begin_frame = &VulkanBeginFrame;
        ApiFunctions->end_frame   = &VulkanEndFrame;
        ApiFunctions->on_resize   = &VulkanOnResize;

        return true;
    }

    void VulkanShutdown()
    {
        vkDeviceWaitIdle(context.device.logical);

        VulkanDestroyPipeline(&context, &context.pipeline);
        VulkanDestroyShaderModule(&context, &context.vertex_shader);
        VulkanDestroyShaderModule(&context, &context.fragment_shader);

        vkDestroyFence(context.device.logical, context.render_fence, nullptr);
        vkDestroySemaphore(context.device.logical, context.render_semaphore, nullptr);
        vkDestroySemaphore(context.device.logical, context.present_semaphore, nullptr);

        VulkanDestroyCommandPool(&context, &context.graphics_pool);

        for (GXuint32 i = 0; i < context.frame_buffers.size(); i++)
        {
            VulkanDestroyFrameBuffer(&context, &context.frame_buffers[i]);
        }

        VulkanDestroyRenderPass(&context, &context.render_pass);
        VulkanDestroySwapchain(&context, &context.swapchain);
        vkDestroySurfaceKHR(context.instance, context.surface, nullptr);
        VulkanDestroyDevice(&context.device);

#if GX_VULKAN_DEBUG
        auto destroyDebugUtilsMessenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkDestroyDebugUtilsMessengerEXT");
        destroyDebugUtilsMessenger(context.instance, context.debug_messenger, nullptr);
#endif // GX_VULKAN_DEBUG

        vkDestroyInstance(context.instance, nullptr);
    }

    VkBool32 VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data)
    {
        if (message_severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            GXE_WARN(callback_data->pMessage);
        }
        else if (message_severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            GXE_ERROR(callback_data->pMessage);
        }

        return VK_FALSE;
    }
}