#include "GXVulkanRenderer.h"
#include "GXVulkanTypes.h"
#include "GXVulkanDevice.h"
#include "GXVulkanSwapChain.h"
#include "GXVulkanRenderPass.h"
#include "GXVulkanCommandBuffers.h"

#include "Logging/Logger.h"
#include "Platform/GXWindow.h"

#include <SDL/SDL_Vulkan.h>

namespace gx {
    
    static GXVulkanContext context;

    static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data);
    
    static void VulkanBeginFrame()
    {
    }

    static void VulkanEndFrame()
    {
    }

    static void VulkanOnResize(GXuint32 Width, GXuint32 Height)
    {
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

        GXE_INFO("Vulkan Renderer Initialized Successfully.");

        ApiFunctions->begin_frame = &VulkanBeginFrame;
        ApiFunctions->end_frame   = &VulkanEndFrame;
        ApiFunctions->on_resize   = &VulkanOnResize;

        return true;
    }

    void VulkanShutdown()
    {
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

    VkBool32 VKAPI_CALL VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data)
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