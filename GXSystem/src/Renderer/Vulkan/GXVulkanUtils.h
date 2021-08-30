#ifndef GX_VULKAN_UTILS_H_
#define GX_VULKAN_UTILS_H_

#include <vulkan/vulkan.h>
#include "Logging/Logger.h"

#define GXVulkanCheck(x) \
    { \
        VkResult result = x; \
        if (result != VK_SUCCESS) { \
            GXE_ERROR("{} => is not successful", #x); \
        } \
    }

#endif